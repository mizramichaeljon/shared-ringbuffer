# shared-ringbuffer

A simple shared-memory ring buffer for passing raw audio samples between processes (e.g., SuperCollider UGen and openFrameworks).

## Features

- Uses Boost.Interprocess
- `SharedRingBuffer(name, size)`
- `write(const float*, numSamples)`
- `close()`

## Example usage in SuperCollider UGen

See the [TapOutUgen project](https://github.com/youruser/TapOutUgen) for a working plugin example.

## Building & Integration

No build required—header-only. Just include:

```cmake
-D SHARED_RINGBUFFER_PATH=/path/to/shared-ringbuffer
```

in the CMake step of the SuperCollider plugin.
