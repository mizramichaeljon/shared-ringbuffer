cmake_minimum_required(VERSION 3.10)
project(SharedRingBuffer)

set(CMAKE_CXX_STANDARD 17)

# === Header-Only Mode ===
include_directories(${CMAKE_SOURCE_DIR}/include)
message(STATUS "Configured SharedRingBuffer as a header-only library.")

# === Optional: CLI tools ===
option(BUILD_RINGBUFFER_TOOLS "Build CLI tools (reader/writer/debug)" ON)

if(BUILD_RINGBUFFER_TOOLS)
    find_package(Boost REQUIRED)

    # === CLI tools ===
    add_executable(circular_reader_test src/circular_reader_test.cpp)
    add_executable(circular_writer_test src/circular_writer_test.cpp)
    add_executable(cleanup src/cleanup.cpp)

    # === Include paths ===
    target_include_directories(circular_reader_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
    )
    target_include_directories(circular_writer_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
    )
    target_include_directories(cleanup PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
    )

    # === Link Boost (safely even if unused) ===
    target_link_libraries(circular_reader_test PRIVATE ${Boost_LIBRARIES})
    target_link_libraries(circular_writer_test PRIVATE ${Boost_LIBRARIES})
    target_link_libraries(cleanup PRIVATE ${Boost_LIBRARIES})
endif()