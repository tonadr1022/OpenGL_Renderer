#!/bin/bash

mkdir -p build-debug
cd build-debug || exit

cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
cd ..
rm compile_commands.json

ln -s ./build-debug/compile_commands.json ./compile_commands.json
./build-debug/opengl_renderer
