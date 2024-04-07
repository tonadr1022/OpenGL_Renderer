if not exist build-release-win (
    mkdir build-release-win
)

cd build-release-win || exit /b

if not exist CMakeCache.txt (
    cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release ..
)

cmake --build . --target opengl_renderer --config Release
cd ..

.\build-release-win\Release\opengl_renderer.exe
