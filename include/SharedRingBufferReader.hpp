#pragma once

#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <cstring>
#include <vector>
#include <atomic>

class SharedRingBufferReader {
public:
    // Constructor with debug output
   SharedRingBufferReader(const std::string& shmName) {
    std::cout << "[SharedRingBufferReader] Attempting to open shared memory: " << shmName << "\n";
    try {
        segment = boost::interprocess::managed_shared_memory(boost::interprocess::open_only, shmName.c_str());
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Could not open shared memory: " << e.what() << "\n";
        std::exit(EXIT_FAILURE);
    }

    base = static_cast<char*>(segment.get_address());
    if (base == nullptr) {
        std::cerr << "[ERROR] segment.get_address() returned nullptr\n";
        std::exit(EXIT_FAILURE);
    }

    std::cout << "[SharedRingBufferReader] Base address obtained: " << static_cast<void*>(base) << "\n";

    try {
        writeIndex = reinterpret_cast<std::atomic<uint32_t>*>(base);
        bufferSizeInSamples = *reinterpret_cast<uint32_t*>(base + 4);
        samples = reinterpret_cast<float*>(base + 8);

        std::cout << "[SharedRingBufferReader] writeIndex: " << writeIndex->load() << "\n";
        std::cout << "[SharedRingBufferReader] bufferSizeInSamples: " << bufferSizeInSamples << "\n";
    } catch (...) {
        std::cerr << "[ERROR] Exception occurred during memory layout parsing\n";
        std::exit(EXIT_FAILURE);
    }
}

    std::vector<float> getLatestSamples(size_t N) {
        uint32_t w = writeIndex->load(std::memory_order_acquire);
        std::cout << "[getLatestSamples] Reading " << N << " samples. Current writeIndex: " << w << "\n";

        std::vector<float> result(N);
        for (size_t i = 0; i < N; ++i) {
            size_t idx = (w + bufferSizeInSamples - N + i) % bufferSizeInSamples;
            result[i] = samples[idx];
        }

        std::cout << "[getLatestSamples] First sample: " << result[0] 
                  << " | Last sample: " << result.back() << "\n";

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