#include "../include/SharedRingBufferWriter.hpp"
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>

int main()
{
    std::cout << "[Main] Entered writer test main()\n";

    constexpr uint32_t sampleRate = 48000;
    constexpr uint32_t bufferSize = sampleRate;

    SharedRingBufferWriter writer("ringbuffer_audio", bufferSize);

    // Generate a 1kHz sine wave for 48000 samples (1 second at 48kHz)
    constexpr float frequency = 1000.0f;
    std::vector<float> sineWave(bufferSize);

    for (size_t i = 0; i < sineWave.size(); ++i)
    {
        sineWave[i] = std::sin(2.0f * M_PI * frequency * (i / static_cast<float>(sampleRate)));
    }

    // Write once into shared memory
    writer.write(sineWave.data(), sineWave.size());
    std::cout << "[Writer] Wrote " << sineWave.size() << " sine samples to shared memory.\n";

    uint64_t globalSampleIndex = 0;
    uint64_t iteration = 0;

    while (true)
    {
        std::vector<float> samples(bufferSize);
        for (uint32_t j = 0; j < bufferSize; ++j)
        {
            float t = static_cast<float>(globalSampleIndex++) / sampleRate;
            samples[j] = std::sin(2.0f * M_PI * 440.0f * t);
        }

        writer.write(samples.data(), samples.size());
        std::cout << "[Writer] Wrote " << bufferSize << " samples to shared memory (iteration " << iteration++ << ")\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}