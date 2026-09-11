# Button Mappings (FGC Mode / XInput)

Pin assignments verified via stm32pintester 2026-03-12.

## Physical Layout

Left hand (movement + utility):
```
  B1(PC0)   B2(PC2)   B3(PA4)
  B4(PD2)   B5(PA7)   B6(PB7)
  B7(PB9)   B8(PB11)  B9(PB1)   B10(PC5)
  B11(PB15) B12(PA7)
```

Right hand (attacks):
```
  B13(PB12) B14(PC8)  B15(PC9)  B16(PC10)  B17(PC11)
  B18(PA5)  B19(PA3)  B20(PA1)  B21(PC3)
  B22(PC12) B23(PB13) B24(PC6)  B25(PB14)
  B26(PC7)  B27(PA0)
```

Shared pins: B5/B12 share PA7 → both mapped to DPad Up (26 unique pins, 27 buttons).
B8/B11 are NOT shared — B8=PB11, B11=PB15.

## Button-to-XInput Mapping

| Button | Pin  | BTN_*    | XInput     | Notes                          |
|--------|------|----------|------------|--------------------------------|
| B1     | PC0  | BTN_MB1  | Start      |                                |
| B2     | PC2  | BTN_RT3  | Select     |                                |
| B3     | PA4  | BTN_RT2  | Home       | Also bootloader entry          |
| B4     | PD2  | —        | —          | Unmapped                       |
| B5     | PA7  | BTN_LT1  | DPad Up    | Shared pin with B12            |
| B6     | PB7  | —        | —          | Unmapped                       |
| B7     | PB9  | BTN_LT2 | L3         |                                |
| B8     | PB11 | BTN_LF3 | DPad Left  |                                |
| B9     | PB1  | BTN_LF2 | DPad Down  |                                |
| B10    | PC5  | BTN_LF1 | DPad Right |                                |
| B11    | PB15 | BTN_LT2  | L3         | Same as B7                     |
| B12    | PA7  | BTN_LT1  | DPad Up    | Shared pin with B5             |
| B13    | PB12 | BTN_RT1  | R3         | Same as B27                    |
| B14    | PC8  | BTN_MB1  | Start      | Same as B1                     |
| B15    | PC9  | BTN_RT3  | Select     | Same as B2                     |
| B16    | PC10 | BTN_RT2  | Home       | Same as B3                     |
| B17    | PC11 | —        | —          | Unmapped                       |
| B18    | PA5  | BTN_RF5 | X          |                                |
| B19    | PA3  | BTN_RF6 | Y          |                                |
| B20    | PA1  | BTN_RF7 | RB         |                                |
| B21    | PC3  | BTN_RF8 | LB         |                                |
| B22    | PC12 | BTN_RF1 | A          |                                |
| B23    | PB13 | BTN_RF2 | B          |                                |
| B24    | PC6  | BTN_RF3 | RT         |                                |
| B25    | PB14 | BTN_RF4 | LT         |                                |
| B26    | PC7  | BTN_LT1 | DPad Up    |                                |
| B27    | PA0  | BTN_RT1 | R3         |                                |

## Unmapped Buttons (Available for Future Use)

B4 (PD2), B6 (PB7), B17 (PC11)

These 3 unique pins (3 buttons) are not mapped to any BTN_* value.

## SOCD Toggle

Hold Start (B1) + Select (B2) + DPad Up (B26) to cycle SOCD modes:
SOCD_NEUTRAL → SOCD_2IP → SOCD_2IP_NO_REAC → SOCD_NEUTRAL
