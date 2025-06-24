#pragma once
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include "SharedRingBufferHeader.hpp"
#include <atomic>
#include <vector>
#include <iostream>

#warning "Compiling SharedRingBufferWriter.hpp"

class SharedRingBufferWriter
{
public:
    SharedRingBufferWriter(const std::string &shmName, uint32_t totalSamples)
        : shmName(shmName), totalSamples(totalSamples)
    {

        std::cout << "[SharedRingBufferWriter] Creating shared memory segment '"
                  << shmName << "' with " << totalSamples << " samples" << std::endl;

        // boost::interprocess::shared_memory_object::remove(shmName.c_str());

        segment = boost::interprocess::managed_shared_memory(
            boost::interprocess::create_only,
            shmName.c_str(),
            calculateSize());

        header = segment.construct<SharedRingBufferHeader>("Header")();
        segment.construct<float>("Samples")[totalSamples](0.0f);

        header->writeIndex.store(0, std::memory_order_release);
        header->bufferSizeInSamples.store(totalSamples, std::memory_order_release);

        std::cout << "[Writer] Header address: " << static_cast<void *>(header)
                  << " | writeIndex=" << header->writeIndex.load()
                  << " | bufferSizeInSamples=" << header->bufferSizeInSamples.load() << std::endl;

        std::cout << "[DEBUG] segment base address: " << segment.get_address() << std::endl;
        std::cout << "[DEBUG] header address: " << static_cast<void *>(header) << std::endl;
        std::cout << "[SharedRingBufferWriter] Shared memory initialized." << std::endl;

        std::cout << "[SharedRingBufferWriter] Shared memory initialized." << std::endl;
    }

    void write(const float *data, size_t count)
    {
        uint32_t w = header->writeIndex.load(std::memory_order_acquire);
        uint32_t size = header->bufferSizeInSamples.load(std::memory_order_acquire);

        std::cout << "[Writer] Writing " << count << " samples at writeIndex=" << w << std::endl;

        float *samples = segment.find<float>("Samples").first;
        for (size_t i = 0; i < count; ++i)
        {
            samples[(w + i) % size] = data[i];
        }
        header->writeIndex.store((w + count) % size, std::memory_order_release);
    }

private:
    std::string shmName;
    uint32_t totalSamples;
    boost::interprocess::managed_shared_memory segment;
    SharedRingBufferHeader *header;

    size_t calculateSize() const
    {
        // Boost metadata + alignment padding
        constexpr size_t padding = 4096;
        return sizeof(SharedRingBufferHeader) + sizeof(float) * totalSamples + padding;
    }
};