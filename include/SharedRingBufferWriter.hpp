#pragma once
#include <boost/interprocess/managed_shared_memory.hpp>
#include "SharedRingBufferHeader.hpp"
#include <vector>
#include <atomic>

class SharedRingBufferReader {
public:
    SharedRingBufferReader(const std::string& shmName) {
        try {
            segment = boost::interprocess::managed_shared_memory(
                boost::interprocess::open_only, shmName.c_str());

            header = segment.find<SharedRingBufferHeader>("Header").first;
        } catch (const std::exception& e) {
            std::exit(EXIT_FAILURE);
        }
    }

    std::vector<float> getLatestSamples(int N) {
        std::vector<float> output(N, 0.0f);

        uint32_t w = header->writeIndex.load(std::memory_order_acquire);
        uint32_t size = header->bufferSizeInSamples.load();

        if (size == 0) return output;

        float* samples = segment.find<float>("Samples").first;
        for (int i = 0; i < N; ++i) {
            int index = (w + size - N + i) % size;
            output[i] = samples[index];
        }
        return output;
    }

    bool hasReceivedValidData() const {
        return header && header->hasWrittenData.load(std::memory_order_acquire);
    }

private:
    mutable boost::interprocess::managed_shared_memory segment;
    SharedRingBufferHeader* header = nullptr;
};