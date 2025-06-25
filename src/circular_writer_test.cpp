#include "../include/SharedRingBufferWriter.hpp"
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <csignal>
#include <atomic>

std::atomic<bool> running(true);

void handle_sigint(int) {
    std::cout << "\n[Signal] SIGINT received. Exiting...\n";
    running = false;
}

int main()
{
    std::signal(SIGINT, handle_sigint);

    std::cout << "[Main] Entered writer test main()\n";

    constexpr uint32_t sampleRate = 48000;
    constexpr uint32_t blockSize = 64;           // Match SC block size
    constexpr uint32_t bufferSize = sampleRate;  // 1 second ring buffer

    SharedRingBufferWriter writer("ringbuffer_audio", bufferSize);

    uint64_t globalSampleIndex = 0;
    uint64_t iteration = 0;

    auto nextWriteTime = std::chrono::steady_clock::now();

    while (running)
    {
        std::vector<float> block(blockSize);
        for (uint32_t i = 0; i < blockSize; ++i)
        {
            float t = static_cast<float>(globalSampleIndex++) / sampleRate;
            block[i] = std::sin(2.0f * M_PI * 440.0f * t);
        }

        writer.write(block.data(), block.size());
        std::cout << "[Writer] Wrote block " << iteration++ << " (" << blockSize << " samples)\n";

        nextWriteTime += std::chrono::microseconds((1'000'000 * blockSize) / sampleRate);
        std::this_thread::sleep_until(nextWriteTime);
    }

    std::cout << "[Main] Writer exited cleanly.\n";
    return 0;
}