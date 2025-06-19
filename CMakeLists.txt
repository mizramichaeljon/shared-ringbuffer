cmake_minimum_required(VERSION 3.10)
project(SharedRingBufferExample)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Find libsndfile
find_library(SNDFILE_LIBRARY sndfile REQUIRED)
# Core headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Tool headers
include_directories(${CMAKE_SOURCE_DIR}/tools)

# === Executables ===
add_executable(ringbuffer_reader
    src/ringbuffer_reader.cpp
    include/SharedRingBufferReader.hpp
)

add_executable(write_test
    src/ringbuffer_write_test.cpp
    include/SharedRingBuffer.hpp
)

add_executable(read_test
    src/ringbuffer_read_test.cpp
    include/SharedRingBufferReader.hpp
)

add_executable(export_test
    src/export_test.cpp
)

target_link_libraries(export_test ${SNDFILE_LIBRARY})
# No extra linking needed for header-only Boost.Interprocess