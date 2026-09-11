import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess

from elftools.elf.elffile import ELFFile


def symbol_bytes(elf, name):
    symbols = elf.get_section_by_name(".symtab").get_symbol_by_name(name)
    if not symbols or len(symbols) != 1:
        raise RuntimeError(f"Expected one ELF symbol: {name}")
    symbol = symbols[0]
    section = elf.get_section(symbol["st_shndx"])
    offset = symbol["st_value"] - section["sh_addr"]
    return section.data()[offset : offset + symbol["st_size"]]


def endpoint_intervals(descriptor):
    intervals = {}
    offset = 0
    while offset < len(descriptor):
        length, kind = descriptor[offset : offset + 2]
        if length < 2 or offset + length > len(descriptor):
            raise RuntimeError("Invalid linked USB descriptor length")
        if kind == 5:
            address = descriptor[offset + 2]
            direction = "IN" if address & 0x80 else "OUT"
            if direction in intervals:
                raise RuntimeError(f"Duplicate {direction} endpoint")
            intervals[direction] = descriptor[offset + 6]
        offset += length
    return intervals


def verify_report_path(elf, path, objdump, baseline):
    symbols = elf.get_section_by_name(".symtab")
    function = next(
        symbol for symbol in symbols.iter_symbols()
        if symbol.name == "_ZN13XInputBackend10SendReportEv"
    )
    address = function["st_value"] & ~1
    assembly = subprocess.check_output(
        [
            objdump, "-d", "-C", f"--start-address={address}",
            f"--stop-address={address + function['st_size']}", str(path),
        ],
        text=True,
    )
    targets = set(re.findall(r"\b(?:bl|b\.w)\s+[0-9a-f]+\s+<([^>]+)>", assembly))
    blocking = "USBXBox360Controller::send()" in targets
    nonblocking = "x360_tx" in targets
    if baseline:
        assert blocking and not nonblocking, targets
    else:
        assert nonblocking and not blocking, targets
    return "blocking" if blocking else "non-blocking"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("elf", type=Path)
    parser.add_argument("--objdump", default="arm-none-eabi-objdump")
    parser.add_argument("--baseline", action="store_true")
    args = parser.parse_args()
    with args.elf.open("rb") as stream:
        elf = ELFFile(stream)
        intervals = endpoint_intervals(symbol_bytes(elf, "X360Descriptor_Config"))
        assert intervals == {"IN": 4 if args.baseline else 1, "OUT": 8}, intervals
        text = elf.get_section_by_name(".text")
        assert text is not None, "ELF has no .text section"
        assert text["sh_addr"] == 0x08001000, text["sh_addr"]
        stack, reset = struct.unpack("<II", text.data()[:8])
        assert stack == 0x20017FF8, hex(stack)
        assert reset & 1 and 0x08001000 <= reset < 0x08080000, hex(reset)
        report_path = verify_report_path(elf, args.elf, args.objdump, args.baseline)
    print(json.dumps({
        "elf": str(args.elf),
        "sha256": hashlib.sha256(args.elf.read_bytes()).hexdigest(),
        "endpoint_intervals_ms": intervals,
        "initial_stack": hex(stack),
        "reset_handler": hex(reset),
        "report_path": report_path,
    }, indent=2))


if __name__ == "__main__":
    main()
