Import("env")
import os

# Replace the default VECT_TAB_ADDR=0x8000000 (set by maple build script)
# with 0x08001000 for our custom bootloader offset.
cppdefines = env.get("CPPDEFINES", [])
new_defines = []
for d in cppdefines:
    if isinstance(d, tuple) and d[0] == "VECT_TAB_ADDR":
        new_defines.append(("VECT_TAB_ADDR", "0x08001000"))
    elif isinstance(d, list) and len(d) == 2 and d[0] == "VECT_TAB_ADDR":
        new_defines.append(["VECT_TAB_ADDR", "0x08001000"])
    else:
        new_defines.append(d)
env.Replace(CPPDEFINES=new_defines)

# Add our config dir to linker search path for the custom .ld file
ld_dir = os.path.join(env["PROJECT_DIR"], "config", "jz-stm32-leverless")
env.Prepend(LIBPATH=[ld_dir])
