if not exist build-debug-win (
    mkdir build-debug-win
)

cd build-debug-win || exit /b

rem Get the number of cores from the environment variable
set "NUM_CORES=%NUMBER_OF_PROCESSORS%"

cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --target opengl_renderer --config Debug -- /maxcpucount:%NUM_CORES%
cd ..

.\build-debug-win\Debug\opengl_renderer.exe
