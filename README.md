# SharedRingBuffer

A lightweight C++ header-only ring buffer implementation using shared memory. Designed for **real-time audio IPC** between processes (e.g. writing from a SuperCollider plugin, reading in an OpenFrameworks visualizer).

## Features

- **Header-only** design (easy to include)
- Built on `boost::interprocess`
- Lock-free writes using atomics
- CLI tools for testing/debugging

## Requirements

- C++17 compiler
- Boost (for CLI tools only)
- CMake ≥ 3.10 (for building CLI tools)

---

## 🔧 Building CLI Tools

### macOS / Linux

Install dependencies via [Homebrew](https://brew.sh) (or your distro’s package manager):

```bash
brew install cmake boost
```

Then build:

```bash
git clone https://github.com/mizramichaeljon/shared-ringbuffer.git
cd shared-ringbuffer
chmod +x build_macos.sh
./build_macos.sh
```

### Windows

Install [CMake](https://cmake.org/) and [Boost](https://www.boost.org/).

We recommend using [vcpkg](https://github.com/microsoft/vcpkg) to install Boost:

```bash
vcpkg install boost
```

Then build:

```cmd
git clone https://github.com/mizramichaeljon/shared-ringbuffer.git
cd shared-ringbuffer
build_windows.bat
```

This builds:
- `circular_reader_test` – continuously reads shared memory
- `circular_writer_test` – emits dummy sine data
- `cleanup` – removes the shared memory segment (useful for debugging)

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

## 🧪 Tests & CLI Utilities

Located in `src/`, but **not required** for core use:

- `circular_writer_test.cpp` — emits sine waves in blocks
- `circular_reader_test.cpp` — reads and prints values
- `cleanup.cpp` — removes shared memory segment (useful for debugging)

---

## 👀 Examples

To see the ring buffer in use:

- 🔉 SuperCollider UGen writer: [TapOutUgen](https://github.com/mizramichaeljon/TapOutUgen)
- 📈 Visualizer built with OpenFrameworks: [ringBufferVisual](https://github.com/mizramichaeljon/ringBufferVisual)

---

## 📦 License

MIT