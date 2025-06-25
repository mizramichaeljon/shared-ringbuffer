# shared-ringbuffer

A lightweight C++ header-only shared-memory ring buffer for passing raw audio samples between processes — e.g. from a SuperCollider UGen to openFrameworks.

## ✨ Features

- **Header-only**: Easy to include and integrate in any C++ project
- **Real-time friendly**: Lock-free write/read operations using atomic indices
- **Cross-process**: Uses Boost.Interprocess to create a shared memory segment accessible across applications
- **Designed for audio**: Buffers raw `float` audio data; intended for block-based audio systems like SuperCollider

## 📦 Components

- `SharedRingBufferHeader.hpp`: Defines shared atomic state (`writeIndex`, `bufferSizeInSamples`)
- `SharedRingBufferWriter.hpp`: Creates and writes to the shared buffer
- `SharedRingBufferReader.hpp`: Reads from the shared buffer in a circular (wrap-around) manner

## 🛠️ Usage

### Writer Setup

```cpp
SharedRingBufferWriter writer("ringbuffer_audio", 48000); // 1 second buffer @ 48kHz

std::vector<float> block(64); // SuperCollider block size

// Fill block with audio samples...
writer.write(block.data(), block.size());
```

### Reader Setup

```cpp
SharedRingBufferReader reader("ringbuffer_audio");

std::vector<float> recentSamples = reader.getLatestSamples(1024); // e.g., 1024 most recent samples
```

## 🔗 Integration

Add the `include/` directory to your project and include the relevant header(s). No compilation needed.

### CMake Example (SuperCollider plugin)

```cmake
target_include_directories(MyUGen PRIVATE ${SHARED_RINGBUFFER_PATH}/include)
```

## ✅ Tested with

- SuperCollider custom UGens (block-size based writes)
- openFrameworks (realtime visualisation via reads)
- Independent test tools for debugging and plotting

## 📂 Optional Test Utilities

Located in `src/`, but **not required** for core use:

- `circular_writer_test.cpp` — emits sine waves in blocks
- `circular_reader_test.cpp` — reads and prints values
- `cleanup.cpp` — removes shared memory segment (useful for debugging)

## 📎 Notes

- Boost is required at runtime for shared memory support
- Make sure to call the writer before the reader to ensure the memory segment is initialized
- For debugging, test tools can be kept in a `dev-ringbuffer-tests` branch

## 🧹 Cleanup

If your system retains a stale shared memory block after crashes:

```sh
ipcrm -M 0x...
```

Or use the included `cleanup.cpp` tool.
