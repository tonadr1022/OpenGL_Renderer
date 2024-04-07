#!/bin/bash

mkdir -p build-debug
cd build-debug || exit

cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
cd ..

./build-debug/opengl_renderer
