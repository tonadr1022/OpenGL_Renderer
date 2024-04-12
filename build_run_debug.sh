#!/bin/bash

# Detect the number of CPU cores
if [ "$(uname)" = "Linux" ]; then
	NUM_CORES=$(nproc)
elif [ "$(uname)" = "Darwin" ]; then
	NUM_CORES=$(sysctl -n hw.ncpu)
else
	echo "Unsupported operating system."
	exit 1
fi

mkdir -p build-debug
cd build-debug || exit

cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build using parallel jobs
cmake --build . -- -j$NUM_CORES

cd ..
ln -s ./build-debug/compile_commands.json ./compile_commands.json

./build-debug/opengl_renderer
