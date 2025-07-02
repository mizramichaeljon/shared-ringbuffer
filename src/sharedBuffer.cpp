
#include "SharedRingBufferHeader.hpp"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <csignal>
#include <iostream>
#include <thread>
#include <chrono>
#include "cxxopts.hpp"

namespace bip = boost::interprocess;

static std::string shmName;
static bool running = true;

void handle_signal(int) {
    running = false;
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("sharedBuffer", "Creates and manages a shared memory audio ring buffer");

    options.add_options()
        ("n,name", "Shared memory name", cxxopts::value<std::string>()->default_value("ringbuffer_audio"))
        ("s,sample-rate", "Sample rate", cxxopts::value<uint32_t>()->default_value("48000"))
        ("d,duration", "Duration in seconds", cxxopts::value<float>()->default_value("2.0"))
        ("c,channels", "Number of channels", cxxopts::value<uint32_t>()->default_value("2"))
        ("h,help", "Print help");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    shmName = result["name"].as<std::string>();
    uint32_t sampleRate = result["sample-rate"].as<uint32_t>();
    float duration = result["duration"].as<float>();
    uint32_t channels = result["channels"].as<uint32_t>();
    std::size_t totalSamples = static_cast<std::size_t>(sampleRate * duration * channels);
    std::size_t segmentSize = totalSamples * sizeof(float) + 1024 * 1024;

    try {
        bip::shared_memory_object::remove(shmName.c_str());
        bip::managed_shared_memory segment(bip::create_only, shmName.c_str(), segmentSize);

        auto header = segment.construct<SharedRingBufferHeader>("Header")();
        header->writeIndex.store(0);
        header->bufferSizeInSamples.store(totalSamples);
        header->sampleRate.store(sampleRate);
        header->numChannels.store(channels);

        segment.construct<float>("Samples")[totalSamples](0.0f);

        std::cout << "[sharedBuffer] Shared memory created: " << shmName << std::endl;
        std::cout << "  Sample rate: " << sampleRate << "\n  Duration: " << duration
                  << "s\n  Channels: " << channels << "\n  Total samples: " << totalSamples << std::endl;

        std::signal(SIGINT, handle_signal);
        std::signal(SIGTERM, handle_signal);

        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        bip::shared_memory_object::remove(shmName.c_str());
        std::cout << "[sharedBuffer] Shared memory removed on exit." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[sharedBuffer] ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
