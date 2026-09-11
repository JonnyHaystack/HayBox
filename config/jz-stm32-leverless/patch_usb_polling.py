from pathlib import Path
import re


def interval_field(source, endpoint, allowed):
    pattern = r"\." + endpoint + r"\s*=\s*\{[^{}]*?\.bInterval\s*=\s*(\d+)\s*,"
    matches = list(re.finditer(pattern, source))
    if len(matches) != 1:
        raise RuntimeError(f"USBComposite: expected one {endpoint} interval")
    match = matches[0]
    if match.group(1) not in allowed:
        raise RuntimeError(f"USBComposite: unexpected {endpoint} interval {match.group(1)}")
    return match.span(1)


globals()["Import"]("env")
env = globals()["env"]

# USBXBox360 uses the wired descriptor; keep host output polling unchanged.
target = (
    Path(env.subst("$PROJECT_LIBDEPS_DIR"))
    / env["PIOENV"]
    / "USBComposite for STM32F1"
    / "usb_multi_x360.c"
)
source = target.read_text()
start, end = interval_field(source, "DataInEndpoint", {"1", "4"})
interval_field(source, "DataOutEndpoint", {"8"})
patched = source[:start] + "1" + source[end:]
if patched != source:
    target.write_text(patched)
print("Verified wired XInput polling: IN 1 ms, OUT 8 ms")
