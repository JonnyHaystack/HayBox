import subprocess

Import("env")


def before_build(source, target, env):
    subprocess.run(["git", "config", "--global", "core.longpaths", "true"])


env.AddPreAction("buildprog", before_build)
env.AddPreAction("upload", before_build)