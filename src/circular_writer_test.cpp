#include "../include/SharedRingBufferWriter.hpp"
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>

int main() {
    std::cout << "[Main] Entered writer test main()\n";

    constexpr uint32_t sampleRate = 48000;
    constexpr uint32_t bufferSize = sampleRate;

    SharedRingBufferWriter writer("ringbuffer_audio", bufferSize);

    uint64_t iteration = 0;
    while (true) {
        std::vector<float> samples(bufferSize);
        for (uint32_t j = 0; j < bufferSize; ++j) {
            samples[j] = std::sin(2 * M_PI * 440.0f * j / sampleRate);
        }

        writer.write(samples.data(), samples.size());
        std::cout << "[Writer] Wrote " << bufferSize << " samples to shared memory (iteration " << iteration++ << ")\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}