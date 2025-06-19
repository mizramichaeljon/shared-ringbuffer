#include "../include/SharedRingBuffer.hpp"
#include <thread>
#include <chrono>
#include <cmath>

int main() {
    constexpr int sampleRate = 48000;
    constexpr int durationSeconds = 1;
    constexpr int numSamples = sampleRate * durationSeconds;

    SharedRingBuffer buffer("ringbuffer_audio", numSamples * sizeof(float));

    float testData[numSamples];
    for (int i = 0; i < numSamples; ++i) {
        testData[i] = std::sin(2 * M_PI * 440.0 * i / sampleRate);  // simple 440Hz sine wave
    }

    std::cout << "[Dummy] Writing sine wave data..." << std::endl;
    buffer.write(testData, numSamples);

    std::cout << "[Dummy] Done. Sleeping 5 seconds before closing." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    buffer.close();
    return 0;
}