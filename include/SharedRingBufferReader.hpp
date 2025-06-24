#pragma once

#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <cstring>
#include <vector>
#include <atomic>

class SharedRingBufferReader {
public:
    // New constructor
    SharedRingBufferReader(const std::string& shmName) {
        segment = boost::interprocess::managed_shared_memory(boost::interprocess::open_only, shmName.c_str());

        base = static_cast<char*>(segment.get_address());
        writeIndex = reinterpret_cast<std::atomic<uint32_t>*>(base);
        bufferSizeInSamples = *reinterpret_cast<uint32_t*>(base + 4);
        samples = reinterpret_cast<float*>(base + 8);

        std::cout << "[SharedRingBufferReader] Connected to shared memory. Buffer size: "
                  << bufferSizeInSamples << " samples\n";
    }

    std::vector<float> getLatestSamples(size_t N) {
        std::vector<float> result(N);
        uint32_t w = writeIndex->load(std::memory_order_acquire);
        for (size_t i = 0; i < N; ++i) {
            size_t idx = (w + bufferSizeInSamples - N + i) % bufferSizeInSamples;
            result[i] = samples[idx];
        }
        return result;
    }

    size_t bufferSize() const {
        return bufferSizeInSamples;
    }

private:
    boost::interprocess::managed_shared_memory segment;
    char* base;
    std::atomic<uint32_t>* writeIndex;
    uint32_t bufferSizeInSamples;
    float* samples;
};