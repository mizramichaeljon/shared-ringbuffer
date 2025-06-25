cmake_minimum_required(VERSION 3.10)
project(SharedRingBuffer)

set(CMAKE_CXX_STANDARD 17)

# === Header-Only Mode ===
include_directories(${CMAKE_SOURCE_DIR}/include)
message(STATUS "Configured SharedRingBuffer as a header-only library.")

# === Optional: CLI tool ===
option(BUILD_RINGBUFFER_READER "Build the CLI ringbuffer reader" ON)

if(BUILD_RINGBUFFER_READER)
    find_package(Boost REQUIRED)

    add_executable(ringbuffer_reader src/ringbuffer_reader.cpp)
    target_include_directories(ringbuffer_reader PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
    )
    target_link_libraries(ringbuffer_reader PRIVATE ${Boost_LIBRARIES})
endif()