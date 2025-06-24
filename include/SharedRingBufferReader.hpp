#pragma once
#include <boost/interprocess/managed_shared_memory.hpp>
#include <atomic>
#include <vector>
#include <iostream>

#warning "Compiling SharedRingBufferReader.hpp"

struct SharedRingBufferHeader {
    std::atomic<uint32_t> writeIndex;
    std::atomic<uint32_t> bufferSizeInSamples;
    alignas(alignof(float)) float samples[];  // Flexible array member
};

class SharedRingBufferReader {
public:
    SharedRingBufferReader(const std::string& shmName) {
        std::cout << "[SharedRingBufferReader] Attempting to open shared memory: " << shmName << std::endl;

        try {
            segment = boost::interprocess::managed_shared_memory(boost::interprocess::open_only, shmName.c_str());
            std::cout << "[SharedRingBufferReader] Shared memory opened successfully." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Could not open shared memory: " << e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        header = static_cast<SharedRingBufferHeader*>(segment.get_address());
        std::cout << "[SharedRingBufferReader] Header loaded: "
                  << "writeIndex=" << header->writeIndex.load()
                  << " | bufferSizeInSamples=" << header->bufferSizeInSamples.load()
                  << std::endl;
    }

    std::vector<float> getLatestSamples(int N) {
        std::vector<float> output(N, 0.0f);

        uint32_t w = header->writeIndex.load(std::memory_order_acquire);
        uint32_t size = header->bufferSizeInSamples.load();

        std::cout << "[getLatestSamples] Reading " << N << " samples at writeIndex=" << w
                  << " bufferSize=" << size << std::endl;

        for (int i = 0; i < N; ++i) {
            int index = (w + size - N + i) % size;
            output[i] = header->samples[index];
        }

        return output;
    }

private:
    boost::interprocess::managed_shared_memory segment;
    SharedRingBufferHeader* header;
};