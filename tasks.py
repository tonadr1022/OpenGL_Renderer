import os
import subprocess
import argparse
import platform

BUILD_DIR = "build-debug"
PROJECT_NAME = "opengl_renderer"
EXECUTABLE_NAME = "opengl_renderer"


def cmake():
    print("Running CMake")
    cmake_command = ["cmake", "-DCMAKE_BUILD_TYPE=Debug", ".."]
    cmake_process = subprocess.Popen(cmake_command)
    cmake_process.wait()

    if cmake_process.returncode != 0:
        print("CMake configuration failed. Exiting...")
        exit(1)


def build():
    print("Building")
    if platform.system() == "Windows":
        parallel_flag = "/m:"
    else:
        parallel_flag = "-j"
    cmake_build_command = ["cmake", "--build", ".", "--", f"{parallel_flag}{str(os.cpu_count())}"]
    build_process = subprocess.Popen(cmake_build_command)
    build_process.wait()

    if build_process.returncode != 0:
        print("Build failed. Exiting...")
        exit(1)


def _find_executable(directory, executable_name):
    for root, dirs, files in os.walk(directory):
        if executable_name in files:
            return os.path.join(root, executable_name)
    return None


def run(executable_name):
    if platform.system() == "Windows":
        executable_name += ".exe"

    executable_path = _find_executable(BUILD_DIR, executable_name)
    if not executable_path:
        print("Error finding executable, exiting")
        exit(1)
    execute_process = subprocess.Popen(executable_path)
    execute_process.wait()

    if execute_process.returncode != 0:
        print("Execution failed. Exiting...")
        exit(1)


def build_run():
    comp_name = "compile_commands.json"
    os.makedirs(BUILD_DIR, exist_ok=True)
    os.chdir(BUILD_DIR)
    cmake()
    build()
    os.chdir("..")
    if os.path.exists(comp_name) or os.path.islink(comp_name):
        print("Remove test")
        os.remove(comp_name)
    os.symlink(
        os.path.join(BUILD_DIR, comp_name), comp_name
    )
    run(EXECUTABLE_NAME)


parser = argparse.ArgumentParser(description=f"{PROJECT_NAME} build tools")

if __name__ == "__main__":
    build_run()
