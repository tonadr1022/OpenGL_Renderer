if not exist build-debug-win (
    mkdir build-debug-win
)
cd build-debug-win || exit /b

cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --target opengl_renderer --config Debug
cd ..

.\build-debug-win\Debug\opengl_renderer.exe