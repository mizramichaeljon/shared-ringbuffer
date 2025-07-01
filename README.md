# SharedRingBuffer

A lightweight C++ header-only ring buffer implementation using shared memory. Designed for **real-time audio IPC** between processes.

## Features

- **Header-only** design (easy to include)
- Built on `boost::interprocess`
- Lock-free writes using atomics
- CLI tools for testing/debugging

---

## Requirements

- C++17-compatible compiler
- Boost (only required to build CLI tools)
- CMake ≥ 3.10 (for building CLI tools)

---

## 🔧 Building CLI Tools

These tools are not required to use the core header files — they are useful for testing shared memory communication.

### macOS / Linux

#### Step 1: Install dependencies

```bash
brew install cmake boost
```

#### Step 2: Clone and build

```bash
git clone https://github.com/mizramichaeljon/shared-ringbuffer.git <path-to-shared-ringbuffer>
cd <path-to-shared-ringbuffer>
mkdir build && cd build
cmake .. -DBUILD_RINGBUFFER_TOOLS=ON
cmake --build .
```

Executables will be located in `./build/`.

---

### Windows

#### Step 1: Install [vcpkg](https://github.com/microsoft/vcpkg)

```cmd
git clone https://github.com/microsoft/vcpkg <path-to-vcpkg>
cd <path-to-vcpkg>
bootstrap-vcpkg.bat
```

#### Step 2: Install Boost

```cmd
vcpkg install boost
```

#### Step 3: Clone and build project

```cmd
git clone https://github.com/mizramichaeljon/shared-ringbuffer.git <path-to-shared-ringbuffer>
cd <path-to-shared-ringbuffer>
mkdir build
cd build

cmake .. -DBUILD_RINGBUFFER_TOOLS=ON -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
```

Executables will be located in `.\build\Release\`.

---

## 🧠 How It Works

The writer and reader communicate via a named shared memory segment. Data is passed using a raw float buffer and a small metadata `struct`.

### Ring Buffer Header Struct:

```cpp
struct SharedRingBufferHeader {
    std::atomic<uint32_t> writeIndex;
    std::atomic<uint32_t> bufferSizeInSamples;
    std::atomic<bool> hasWrittenData;
};
```

---

## ✍️ Writing to Shared Memory

Use the `SharedRingBufferWriter` class. Example:

```cpp
#include "SharedRingBufferWriter.hpp"

SharedRingBufferWriter writer("ringbuffer_audio", 48000); // 1 second buffer @ 48kHz

std::vector<float> block(64); // e.g. a block of audio samples
// Fill block with audio samples...
writer.write(block.data(), block.size());
```

This will automatically create or attach to the shared memory segment and publish data into the ring buffer.

---

## 📖 Reading from Shared Memory

Use the `SharedRingBufferReader` class. Example:

```cpp
#include "SharedRingBufferReader.hpp"

SharedRingBufferReader reader("ringbuffer_audio");

std::vector<float> latest = reader.getLatestSamples(512);
```

Before drawing or processing the data, you can verify validity:

```cpp
if (reader.hasReceivedValidData()) {
    // process data, etc.
}
```

---

## 🧪 CLI Test Utilities

Located in `src/`, but **not required** for normal use:

- `circular_writer_test.cpp` — emits sine waves in blocks
- `circular_reader_test.cpp` — reads and prints values
- `cleanup.cpp` — removes shared memory segment 

---

## 👀 Examples

To see the ring buffer in use:

- 🔉 SuperCollider UGen writer: [TapOutUgen](https://github.com/mizramichaeljon/TapOutUgen)
- 📈 Visualizer built with OpenFrameworks: [ringBufferVisual](https://github.com/mizramichaeljon/ringBufferVisual)

---

## 📦 License

MIT
