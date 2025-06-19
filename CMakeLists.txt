cmake_minimum_required(VERSION 3.10)
project(SharedRingBufferExample)

set(CMAKE_CXX_STANDARD 17)

# This tells CMake to use the old `FindBoost` mode
set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


# Add this line to include your local headers
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(ringbuffer_demo
    src/main.cpp
    include/SharedRingBuffer.hpp
)

add_executable(ringbuffer_reader
    src/reader.cpp
    include/SharedRingBufferReader.hpp
)

target_link_libraries(ringbuffer_reader)

# Since Boost.Interprocess is header-only, no need to link a library
target_link_libraries(ringbuffer_demo)