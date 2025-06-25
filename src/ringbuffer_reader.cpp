#include "../include/SharedRingBufferReader.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <csignal>
#include <atomic>

std::atomic<bool> running(true);

void handle_sigint(int)
{
    std::cout << "\n[Signal] SIGINT received. Exiting...\n";
    running = false;
}

int main()
{
    std::cout << "[Main] Entered ringbuffer real-time reader\n";
    std::signal(SIGINT, handle_sigint); // Handle Ctrl+C

    SharedRingBufferReader reader("ringbuffer_audio");

    constexpr int blockSize = 1024;
    constexpr int pollingIntervalMs = 16; // ~60Hz

    while (running)
    {
        std::vector<float> out = reader.getLatestSamples(blockSize);

        if (out.empty())
        {
            std::cerr << "[Reader] WARNING: Empty sample buffer\n";
        }
        else
        {
            std::cout << "[Reader] First: " << out.front()
                      << ", Last: " << out.back()
                      << ", Size: " << out.size() << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(pollingIntervalMs));
    }

    std::cout << "[Main] Reader exited cleanly.\n";
    return 0;
}