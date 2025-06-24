cmake_minimum_required(VERSION 3.10)
project(SharedRingBufferExample)

set(CMAKE_CXX_STANDARD 17)

# === Boost ===
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# === libsndfile (optional, just found not used yet) ===
find_library(SNDFILE_LIBRARY sndfile)

# === Core Headers ===
include_directories(${CMAKE_SOURCE_DIR}/include)

# === Python/matplotlib-cpp Support ===
# Try to find Python3 with matplotlib first, falling back to system default
find_program(SYSTEM_PYTHON3 NAMES python3 REQUIRED)

# Use that as a default, but allow override via cache
set(Python3_EXECUTABLE "${SYSTEM_PYTHON3}" CACHE FILEPATH "Python interpreter" FORCE)

# Continue finding interpreter/dev/numpy components
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/tools/matplotlib-cpp)

# === Executables ===

# Reader: includes plotting support
add_executable(ringbuffer_reader
    src/ringbuffer_reader.cpp
    include/SharedRingBufferReader.hpp
)

# Circular buffer writer (continuous)
add_executable(circular_writer_test
    src/circular_writer_test.cpp
    include/SharedRingBufferWriter.hpp
)

# Circular buffer reader (with plotting)
add_executable(circular_reader_test
    src/circular_reader_test.cpp
    include/SharedRingBufferReader.hpp
)

# Cleanup tool to remove shared memory segment
add_executable(cleanup_shared_memory
    src/cleanup.cpp
)

# === Link Python libs to plotting readers ===
target_link_libraries(circular_reader_test
    ${Python3_LIBRARIES}
)

target_link_libraries(ringbuffer_reader
    ${Python3_LIBRARIES}
)