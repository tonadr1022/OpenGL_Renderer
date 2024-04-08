if not exist build-release-win (
    mkdir build-release-win
)

cd build-release-win || exit /b

rem Get the number of cores from the environment variable
set "NUM_CORES=%NUMBER_OF_PROCESSORS%"

cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target opengl_renderer --config Release -- /maxcpucount:%NUM_CORES%
cd ..

.\build-release-win\Release\opengl_renderer.exe
