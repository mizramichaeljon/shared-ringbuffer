#!/bin/bash
set -e

echo "Cleaning previous build..."
rm -rf build

echo "Rebuilding..."
mkdir build && cd build
cmake .. -DBUILD_RINGBUFFER_READER=ON
cmake --build .

echo "Done. Executables (if built) are in ./build/"