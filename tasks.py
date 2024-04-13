import os
import subprocess
import argparse

build_dir = "build-debug"
project_name = "opengl_renderer"
executable_name = "opengl_renderer"


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
    cmake_build_command = ["cmake", "--build", ".", "--", "-j", str(os.cpu_count())]
    build_process = subprocess.Popen(cmake_build_command)
    build_process.wait()

    if build_process.returncode != 0:
        print("Build failed. Exiting...")
        exit(1)


def run():
    execute_command = [os.path.join(build_dir, executable_name)]
    execute_process = subprocess.Popen(execute_command)
    execute_process.wait()

    if execute_process.returncode != 0:
        print("Execution failed. Exiting...")
        exit(1)


def build_run():
    os.makedirs(build_dir, exist_ok=True)
    os.chdir(build_dir)
    cmake()
    build()
    os.chdir("..")
    if os.path.exists("compile_commands.json"):
        os.remove("compile_commands.json")
    os.symlink(
        os.path.join(build_dir, "compile_commands.json"), "compile_commands.json"
    )
    run()


parser = argparse.ArgumentParser(description=f"{project_name} build tools")

if __name__ == "__main__":
    build_run()
