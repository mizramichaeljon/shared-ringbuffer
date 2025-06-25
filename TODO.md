# TODO: SharedRingBuffer Integration and TapOutUgen Merge

## ✅ Merge shared-ringbuffer into TapOutUgen
- [ ] Move shared buffer headers and source into TapOutUgen repo (or use as submodule)
- [ ] Replace test audio buffers with `SharedRingBufferReader` in TapOutUGen.cpp
- [ ] Ensure build system (CMake) includes the buffer code

## 🌀 Continuous Reading Mode
- [ ] Convert `circular_reader_test` to support continuous streaming
- [ ] Track last read index and support wraparound reads
- [ ] Prevent duplicate or missed reads in continuous mode

## 🧪 Data Fidelity Test
- [ ] Write unit test that:
  - Writes a known sine wave to buffer
  - Reads from buffer
  - Verifies amplitude and frequency match original (within epsilon)

## 🧼 Shared Memory Cleanup
- [ ] Improve cleanup tool (`cleanup_shared_memory`)
- [ ] Add signal handler to writer to release segment on crash
- [ ] Optional: watchdog tool to monitor stale shared segments

## 📦 CMake Build Integration
- [ ] Move ringbuffer logic to `ringbuffer` module/namespace
- [ ] Add CMake options to include/exclude test apps
- [ ] Link ringbuffer to TapOutUgen and unit test targets

## 📈 Performance Monitoring (Optional)
- [ ] Add logging/timing for:
  - Time between writes
  - Buffer underruns or overruns
  - Missed reads or lag

## 🔀 Thread-Safe TapOutUgen Integration
- [ ] Ensure `getLatestSamples` avoids heap allocation inside audio callback
- [ ] Avoid mutex locks in real-time audio path
- [ ] Preallocate read buffer in TapOutUgen and reuse

---

_This checklist helps finalize the integration of shared ring buffers into TapOutUgen and ensure robust, real-time performance with testability and maintainability._