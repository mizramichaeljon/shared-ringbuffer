#include "../include/SharedRingBufferReader.hpp"
#include <fstream>
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
    std::cout << "[Main] Entered reader test main()\n";
    std::signal(SIGINT, handle_sigint); // Handle Ctrl+C

    SharedRingBufferReader reader("ringbuffer_audio");

    constexpr int sampleRate = 48000;
    constexpr int blockSize = 1024;
    constexpr int pollingIntervalMs = 16; // ~60Hz

    // Open CSV in append mode
    std::ofstream csv("out_combined.csv", std::ios::app);
    if (!csv.is_open()) {
        std::cerr << "[Error] Failed to open out_combined.csv\n";
        return 1;
    }

    while (running)
    {
        std::vector<float> out = reader.getLatestSamples(blockSize);

        if (out.empty()) {
            std::cerr << "[Reader] WARNING: Received empty sample buffer\n";
        } else {
            std::cout << "[Reader] First: " << out.front()
                      << ", Last: " << out.back() << "\n";

            for (float f : out) csv << f << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(pollingIntervalMs));
    }

    csv.close();
    std::cout << "[Main] Reader exited cleanly.\n";
    return 0;
}