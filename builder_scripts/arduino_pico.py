import subprocess

Import("env")


def before_build():
    subprocess.run(["git", "config", "--global", "core.longpaths", "true"])


before_build()