#pragma once

#include <boost/interprocess/managed_shared_memory.hpp>
#include "SharedRingBufferHeader.hpp"
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>

class SharedRingBufferReader
{
public:
    SharedRingBufferReader(const std::string &shmName)
    {
        try
        {
            segment = boost::interprocess::managed_shared_memory(
                boost::interprocess::open_only, shmName.c_str());

            header = segment.find<SharedRingBufferHeader>("Header").first;
        }
        catch (const std::exception &e)
        {
            std::exit(EXIT_FAILURE);
        }
    }

    // Returns latest interleaved frames: [L0, R0, L1, R1, ..., Ln, Rn]
    std::vector<float> getInterleavedFrames(int numFrames)
    {
        uint32_t w = header->writeIndex.load(std::memory_order_acquire);
        uint32_t size = header->bufferSizeInSamples.load(std::memory_order_acquire);
        uint32_t channels = header->numChannels.load(std::memory_order_acquire);

        std::vector<float> output(numFrames * channels, 0.0f);
        if (size == 0 || channels == 0)
            return output;

        float *samples = segment.find<float>("Samples").first;
        for (int i = 0; i < numFrames * channels; ++i)
        {
            int index = (w + size - (numFrames * channels) + i) % size;
            output[i] = samples[index];
        }

        return output;
    }

    // Returns latest frames split by channel: [ [L0, L1, ...], [R0, R1, ...] ]
    std::vector<std::vector<float>> getDeinterleavedFrames(int numFrames)
    {
        uint32_t w = header->writeIndex.load(std::memory_order_acquire);
        uint32_t size = header->bufferSizeInSamples.load(std::memory_order_acquire);
        uint32_t channels = header->numChannels.load(std::memory_order_acquire);

        std::vector<std::vector<float>> output(channels, std::vector<float>(numFrames, 0.0f));
        if (size == 0 || channels == 0)
            return output;

        float *samples = segment.find<float>("Samples").first;

        for (int i = 0; i < numFrames; ++i)
        {
            int frameStart = ((w + size - (numFrames * channels)) + i * channels) % size;
            for (uint32_t ch = 0; ch < channels; ++ch)
            {
                output[ch][i] = samples[(frameStart + ch) % size];
            }
        }

        return output;
    }

    bool hasReceivedValidData() const
    {
        return header && header->hasWrittenData.load(std::memory_order_acquire);
    }

    uint32_t getSampleRate() const
    {
        return header ? header->sampleRate.load(std::memory_order_acquire) : 0;
    }

    uint32_t getNumChannels() const
    {
        return header ? header->numChannels.load(std::memory_order_acquire) : 0;
    }

private:
    mutable boost::interprocess::managed_shared_memory segment;
    SharedRingBufferHeader *header = nullptr;
};
