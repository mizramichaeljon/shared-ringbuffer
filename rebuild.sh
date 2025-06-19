#!/bin/bash
set -e

# Ensure we're in the project root (e.g., TapOutUgen)
if [[ ! -f "CMakeLists.txt" ]]; then
  echo "Error: Must run this script from the project root (where CMakeLists.txt lives)."
  exit 1
fi

echo "Cleaning previous build..."
rm -rf build

echo "Rebuilding..."
mkdir build && cd build
cmake ..
cmake --build .