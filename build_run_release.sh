#!/bin/bash

mkdir -p build-release
cd build || exit

cmake -DCMAKE_BUILD_TYPE=Release ..

cmake --build .
./opengl_renderer
