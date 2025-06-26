#pragma once
#include <atomic>

struct SharedRingBufferHeader {
    std::atomic<uint32_t> writeIndex{0};
    std::atomic<uint32_t> bufferSizeInSamples{0};
    std::atomic<bool> hasWrittenData{false};  // NEW
};