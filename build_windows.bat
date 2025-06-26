@echo off
echo Cleaning previous build...
rmdir /s /q build

echo Rebuilding...
mkdir build
cd build

cmake .. -DBUILD_RINGBUFFER_READER=ON
cmake --build .

echo Done. Executables (if built) are in .\build\