#pragma once
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <atomic>
#include <vector>
#include <iostream>

#warning "Compiling SharedRingBufferWriter.hpp"

struct SharedRingBufferHeader {
    std::atomic<uint32_t> writeIndex;
    std::atomic<uint32_t> bufferSizeInSamples;
    alignas(alignof(float)) float samples[];  // Flexible array member
};

class SharedRingBufferWriter {
public:
    SharedRingBufferWriter(const std::string& shmName, uint32_t totalSamples)
        : shmName(shmName), totalSamples(totalSamples) {
        
        std::cout << "[SharedRingBufferWriter] Creating shared memory segment '" 
                  << shmName << "' with " << totalSamples << " samples" << std::endl;

        boost::interprocess::shared_memory_object::remove(shmName.c_str());

        segment = boost::interprocess::managed_shared_memory(
            boost::interprocess::create_only,
            shmName.c_str(),
            calculateSize()
        );

        void* baseAddr = segment.allocate(sizeof(SharedRingBufferHeader) + sizeof(float) * totalSamples);
        header = new (baseAddr) SharedRingBufferHeader;

        header->writeIndex.store(0, std::memory_order_release);
        header->bufferSizeInSamples.store(totalSamples, std::memory_order_release);

        std::cout << "[SharedRingBufferWriter] Shared memory initialized." << std::endl;
    }

    void write(const float* data, size_t count) {
        uint32_t w = header->writeIndex.load(std::memory_order_acquire);
        uint32_t size = header->bufferSizeInSamples.load(std::memory_order_acquire);

        std::cout << "[Writer] Writing " << count << " samples at writeIndex=" << w << std::endl;

        for (size_t i = 0; i < count; ++i) {
            header->samples[(w + i) % size] = data[i];
        }

        header->writeIndex.store((w + count) % size, std::memory_order_release);
    }

private:
    std::string shmName;
    uint32_t totalSamples;
    boost::interprocess::managed_shared_memory segment;
    SharedRingBufferHeader* header;

    size_t calculateSize() const {
        return sizeof(SharedRingBufferHeader) + sizeof(float) * totalSamples + 4096; // small padding
    }
};