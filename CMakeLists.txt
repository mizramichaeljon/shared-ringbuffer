cmake_minimum_required(VERSION 3.10)
project(SharedRingBuffer)

set(CMAKE_CXX_STANDARD 17)

# === Headers-Only Library ===
# This project provides only header files for inclusion in other C++ projects.

# Add the include directory to the project
include_directories(${CMAKE_SOURCE_DIR}/include)

# Optional: install header files for system-wide use
# install(DIRECTORY include/ DESTINATION include)

message(STATUS "Configured SharedRingBuffer as a header-only library.")