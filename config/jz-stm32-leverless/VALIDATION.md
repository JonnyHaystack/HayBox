# STM32 USB validation

## Build and checks

Use a Python virtual environment with PlatformIO 6.1.19.
The STM32 environment pins its platform, compiler, framework, and libraries in `platformio.ini`.

```sh
python -m pip install platformio==6.1.19 -r tests/requirements.txt
pio run -e jz-stm32-leverless
python -m unittest discover -s tests -p test_stm32_usb_polling.py
python tests/verify_stm32_firmware.py .pio/build/jz-stm32-leverless/firmware.elf \
  --objdump "${PLATFORMIO_CORE_DIR:-$HOME/.platformio}/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-objdump"
```

Run the ELF checker with `--baseline` for the unchanged `184a86f` build.
The checker requires the pinned compiler's disassembler and an unstripped ELF file.
It checks linked descriptor intervals, report submission calls, the application address, and the initial stack pointer.
These checks do not measure physical input latency.

Generate `compile_commands.json` with `pio run -e jz-stm32-leverless -t compiledb` for editor include paths.
Build again after this target before checking the ELF file.
The compilation database captures vector definitions before the existing post-build configuration script applies its override.
Use the linked startup instructions to verify the runtime vector address.

## Verified comparison

| Property | Rebuilt baseline `184a86f` | First USB candidate |
| --- | --- | --- |
| Linked wired IN interval | 4 ms | 1 ms |
| Linked wired OUT interval | 8 ms | 8 ms |
| Backend submission call | `USBXBox360Controller::send()` | `x360_tx()` |
| Flash usage, text plus data | 53,336 bytes | 53,364 bytes |
| Static RAM, data plus BSS | 6,176 bytes | 6,176 bytes |
| Application address | `0x08001000` | `0x08001000` |
| Initial stack pointer | `0x20017ff8` | `0x20017ff8` |

All seven descriptor regression tests fail against the original patch script and pass against the replacement.
A build with fresh library dependencies passes.
The candidate's preprocessed USB source defines `BOARD_USB_DISC_DEV` as `NULL`.
The linked startup passes `0x08001000` to `nvic_init`.
Pyright passes with the verification environment selected.
For the first USB change, Lizard reports a maximum complexity of seven.

## Device check

Windows binds the controller to the built-in Xbox 360 driver through its per-device driver picker.
The DFU download writes 53,364 application bytes starting at `0x08001000`.
The update does not write the first 4 KiB bootloader region.
The device remains in DFU after the leave request during this test.
A normal power cycle starts the candidate and restores Windows XInput on slot zero.
B22/PC12 reports the A button as `0x1000` while held.
Releasing B22 restores the button mask to `0x0000`.

The application readback attempt returns zero bytes.
No usable device backup exists from that attempt.
The rebuilt baseline is a source comparison artifact, not a byte-for-byte copy of the previous application.

## SOCD mode-configuration correction

The FGC toggle updates its configuration in place without rebinding the active mode.
The corrected image uses 53,340 bytes of flash and 6,176 bytes of static RAM.
Disassembly shows the loop's mode-rebinding call before the fix and its absence after the fix.
The compiled loop shrinks from 196 to 172 bytes.
Lizard reports a loop complexity of nine.
The seven descriptor tests and the ELF checks pass for the corrected build.

The corrected application is flashed at `0x08001000` and starts after a normal power cycle.
The live regression sequence selects Ultimate, cycles FGC SOCD, then presses Left followed by Right while holding both.
XInput reports LX = 25,828, confirming Right wins instead of resolving to neutral at 128.
A subsequent power cycle restores the default FGC configuration before the input capture.

## Windows A-button capture

A 20-second capture samples XInput 79,006 times during ten requested slow taps.
It records ten presses and ten releases, with no additional A-button transitions.
The trace contains no XInput errors or other button inputs.
It begins and ends with all buttons released.
Reported holds range from 110.962 to 156.052 ms.
The shortest reported interval between a release and the next press is 789.208 ms.
The host sampler's largest polling gap is 0.4586 ms; its p99 gap is 0.2584 ms.
These gap values measure the host sampler's cadence.
Physical input latency and electrical contact bounce remain unmeasured.

## Remaining acceptance checks

- Exercise every mapped button, simultaneous directions, and independent releases of duplicate actions.
- Verify suspend/resume and a sustained input workload.
- Measure physical input-to-report timing with a correlated timebase.
- Measure mechanical contact bounce before selecting a debounce policy.
- Specify the minimum supported pulse width and test pulses between USB polls.
- Exercise every SOCD policy and its direction-release sequences.

Input scanning continues while the endpoint is busy.
The next successful submission uses the latest sampled state.
A press and release entirely between successful reports can remain unreported.
The candidate does not add pulse latching or mechanical debounce.
Graphical configuration and multiple profiles remain follow-up work.

## Bootloader follow-up

Retain the current 4 KiB DFU design for this board.
Evaluate `ENABLE_DFU_UPLOAD` for backups and flash verification.
Reconcile the advertised 1 KiB erase regions with the erase code's 2 KiB page handling.
Evaluate image validation together with its required application metadata.
These changes require separate bootloader work and testing.
