cmake_minimum_required(VERSION 3.10)
project(SharedRingBufferExample)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Find libsndfile
find_library(SNDFILE_LIBRARY sndfile REQUIRED)

# Core headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Python/matplotlib support
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})

# matplotlib-cpp header
include_directories(${CMAKE_SOURCE_DIR}/tools/matplotlib-cpp)

# === Executables ===

add_executable(ringbuffer_reader
    src/ringbuffer_reader.cpp
    include/SharedRingBufferReader.hpp
)

add_executable(circular_writer_test
    src/circular_writer_test.cpp
    include/SharedRingBufferWriter.hpp
)

add_executable(circular_reader_test
    src/circular_reader_test.cpp
    include/SharedRingBufferReader.hpp
)

target_link_libraries(circular_reader_test
    ${Python3_LIBRARIES}
)