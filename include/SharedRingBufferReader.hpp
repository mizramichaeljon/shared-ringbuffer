#pragma once
#include <boost/interprocess/managed_shared_memory.hpp>
#include "SharedRingBufferHeader.hpp"
#include <atomic>
#include <vector>
#include <iostream>
#include <thread> // <-- Add this
#include <chrono> // Already included transitively by <thread>, but explicit is fine
#warning "Compiling SharedRingBufferReader.hpp"

class SharedRingBufferReader
{
public:
    SharedRingBufferReader(const std::string &shmName)
    {
        std::cout << "[SharedRingBufferReader] Attempting to open shared memory: " << shmName << std::endl;

        try
        {
            segment = boost::interprocess::managed_shared_memory(boost::interprocess::open_only, shmName.c_str());
            std::cout << "[SharedRingBufferReader] Shared memory opened successfully." << std::endl;

            header = segment.find<SharedRingBufferHeader>("Header").first;

            /*
             int waitCount = 0;
            while (header->bufferSizeInSamples.load(std::memory_order_acquire) == 0)
            {
                if (waitCount++ > 1000)
                {
                    std::cerr << "[Reader] ERROR: Timed out waiting for writer to initialize shared memory.\n";
                    std::exit(EXIT_FAILURE);
                }


            td::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
*/
            std::cout << "[DEBUG] segment base address: " << segment.get_address() << std::endl;
            std::cout << "[DEBUG] header address: " << static_cast<void *>(header) << std::endl;

            std::cout << "[Reader] Header address: " << static_cast<void *>(header)
                      << " | writeIndex=" << header->writeIndex.load()
                      << " | bufferSizeInSamples=" << header->bufferSizeInSamples.load() << std::endl;

            std::cout << "[SharedRingBufferReader] Header loaded: "
                      << "writeIndex=" << header->writeIndex.load()
                      << " | bufferSizeInSamples=" << header->bufferSizeInSamples.load()
                      << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR] Could not open shared memory: " << e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    std::vector<float>
    getLatestSamples(int N)
    {
        std::vector<float> output(N, 0.0f);

        uint32_t w = header->writeIndex.load(std::memory_order_acquire);
        uint32_t size = header->bufferSizeInSamples.load();

        std::cout << "[getLatestSamples] Reading " << N << " samples at writeIndex=" << w
                  << " bufferSize=" << size << std::endl;

        if (size == 0)
            return output; // Prevent division by 0

        float *samples = segment.find<float>("Samples").first;
        for (int i = 0; i < N; ++i)
        {
            int index = (w + size - N + i) % size;
            output[i] = samples[index];
        }
        return output;
    }

private:
    boost::interprocess::managed_shared_memory segment;
    SharedRingBufferHeader *header = nullptr;
};