
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0167 NEW)

project(SharedRingBuffer)

set(CMAKE_CXX_STANDARD 17)

# === Header-Only Mode ===
include_directories(${CMAKE_SOURCE_DIR}/include)
message(STATUS "Configured SharedRingBuffer as a header-only library.")

# === Optional: CLI tools ===
option(BUILD_RINGBUFFER_TOOLS "Build CLI tools (reader/writer/debug)" ON)

if(BUILD_RINGBUFFER_TOOLS)
    find_package(Boost REQUIRED COMPONENTS interprocess)

    # === CLI tools ===
    add_executable(circular_reader_test src/circular_reader_test.cpp)
    add_executable(circular_writer_test src/circular_writer_test.cpp)
    add_executable(cleanup src/cleanup.cpp)
    add_executable(sharedBuffer src/sharedBuffer.cpp)

    # === Include paths ===
    foreach(target IN ITEMS circular_reader_test circular_writer_test cleanup sharedBuffer)
        target_include_directories(${target} PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${Boost_INCLUDE_DIRS}
        )
        target_link_libraries(${target} PRIVATE ${Boost_LIBRARIES})
    endforeach()
endif()
