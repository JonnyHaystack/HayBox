import contextlib
import io
from pathlib import Path
import runpy
import tempfile
import unittest


SCRIPT = (
    Path(__file__).resolve().parents[1]
    / "config/jz-stm32-leverless/patch_usb_polling.py"
)
DESCRIPTOR = """\
.DataInEndpoint = {
    .bInterval = 4,
},
.DataOutEndpoint = {
    .bInterval = 8,
},
"""


class BuildEnvironment(dict):
    def subst(self, value):
        return self[value]


class UsbPollingTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name)
        self.env = BuildEnvironment(
            PIOENV="jz-stm32-leverless",
            **{"$PROJECT_LIBDEPS_DIR": str(root)},
        )
        package = root / self.env["PIOENV"] / "USBComposite for STM32F1"
        package.mkdir(parents=True)
        self.wired = package / "usb_multi_x360.c"
        self.wireless = package / "usb_x360w.c"
        self.wired.write_text(DESCRIPTOR)
        self.wireless.write_text(DESCRIPTOR)

    def run_patch(self):
        with contextlib.redirect_stdout(io.StringIO()):
            runpy.run_path(
                str(SCRIPT),
                init_globals={"env": self.env, "Import": lambda name: None},
            )

    def test_patches_only_wired_input_interval(self):
        self.run_patch()
        self.assertEqual(
            self.wired.read_text(), DESCRIPTOR.replace("bInterval = 4", "bInterval = 1")
        )
        self.assertEqual(self.wireless.read_text(), DESCRIPTOR)

    def test_repeated_build_preserves_patched_file(self):
        self.run_patch()
        modified = self.wired.stat().st_mtime_ns
        self.run_patch()
        self.assertEqual(self.wired.stat().st_mtime_ns, modified)
        self.assertIn("bInterval = 1", self.wired.read_text())

    def test_missing_wired_source_fails_build(self):
        self.env["PIOENV"] = "missing-environment"
        with self.assertRaises(FileNotFoundError):
            self.run_patch()

    def assert_rejected(self, source):
        self.wired.write_text(source)
        with self.assertRaises(RuntimeError):
            self.run_patch()
        self.assertEqual(self.wired.read_text(), source)

    def test_missing_input_field_fails_build(self):
        self.assert_rejected(DESCRIPTOR.replace("DataInEndpoint", "RenamedEndpoint"))

    def test_duplicate_input_field_fails_build(self):
        self.assert_rejected(DESCRIPTOR + ".DataInEndpoint = { .bInterval = 4, },\n")

    def test_unexpected_input_interval_fails_build(self):
        self.assert_rejected(DESCRIPTOR.replace("bInterval = 4", "bInterval = 2"))

    def test_unexpected_output_interval_fails_build(self):
        self.assert_rejected(DESCRIPTOR.replace("bInterval = 8", "bInterval = 4"))


if __name__ == "__main__":
    unittest.main()
