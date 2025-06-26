#pragma once

#include "SharedRingBufferHeader.hpp"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <iostream>
#include <stdexcept>
#include <cstring>

namespace bip = boost::interprocess;

class SharedRingBufferWriter {
public:
    SharedRingBufferWriter(const std::string& name, std::size_t numSamples)
        : shmName(name), totalSamples(numSamples)
    {
        try {
            // Attempt to create shared memory segment
            segment = bip::managed_shared_memory(bip::create_only, shmName.c_str(), calculateSize());

            // Create and initialize header
            header = segment.construct<SharedRingBufferHeader>("Header")();
            header->writeIndex.store(0, std::memory_order_release);
            header->bufferSizeInSamples.store(totalSamples, std::memory_order_release);

            // Create and zero-initialize buffer
            segment.construct<float>("Samples")[totalSamples](0.0f);

            std::cout << "[Writer] Created new shared memory segment: " << shmName << "\n";
        } catch (const bip::interprocess_exception& e) {
            std::cerr << "[Writer] Shared memory exists. Attaching instead: " << e.what() << "\n";

            // Open existing segment instead of failing
            segment = bip::managed_shared_memory(bip::open_only, shmName.c_str());

            // Get header and buffer
            header = segment.find<SharedRingBufferHeader>("Header").first;
            if (!header) throw std::runtime_error("[Writer] ERROR: Header not found in shared memory.");
        }
    }

    ~SharedRingBufferWriter() {
        // Detach, don't remove
        // Shared memory segment stays alive until explicitly removed
    }

    void write(const float* data, std::size_t count) {
        float* buffer = segment.find<float>("Samples").first;
        if (!buffer) {
            std::cerr << "[Writer] ERROR: Shared buffer not found.\n";
            return;
        }

        std::size_t writeIdx = header->writeIndex.load(std::memory_order_acquire);
        std::size_t bufferSize = header->bufferSizeInSamples.load(std::memory_order_acquire);

        for (std::size_t i = 0; i < count; ++i) {
            buffer[(writeIdx + i) % bufferSize] = data[i];
        }

        header->writeIndex.store((writeIdx + count) % bufferSize, std::memory_order_release);
        header->hasWrittenData.store(true, std::memory_order_release);
    }

private:
    std::string shmName;
    std::size_t totalSamples;

    bip::managed_shared_memory segment;
    SharedRingBufferHeader* header = nullptr;

    std::size_t calculateSize() const {
        // 1MB buffer overhead plus data
        return totalSamples * sizeof(float) + 1024 * 1024;
    }
};
