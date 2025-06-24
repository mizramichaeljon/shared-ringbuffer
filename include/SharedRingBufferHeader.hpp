#pragma once
#include <atomic>

struct SharedRingBufferHeader {
    std::atomic<uint32_t> writeIndex;
    std::atomic<uint32_t> bufferSizeInSamples;
};