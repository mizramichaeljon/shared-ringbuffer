#include "../include/SharedRingBufferWriter.hpp"
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>

int main() {
    std::cout << "[Main] Entered writer test main()\n";

    const char* shmName = "ringbuffer_audio";
    const size_t bufferSizeInSamples = 48000;
    SharedRingBufferWriter writer(shmName, bufferSizeInSamples);

    const double sampleRate = 48000.0;
    const double frequency = 440.0;
    const double twoPiF = 2.0 * M_PI * frequency;

    for (int i = 0; i < 5; ++i) {
        std::vector<float> samples(bufferSizeInSamples);
        for (size_t j = 0; j < bufferSizeInSamples; ++j) {
            samples[j] = static_cast<float>(std::sin(twoPiF * j / sampleRate));
        }

        writer.write(samples.data(), samples.size());
        std::cout << "[Writer] Wrote " << samples.size() << " samples to shared memory (iteration " << i << ")\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "[Writer] Done writing\n";
    return 0;
}