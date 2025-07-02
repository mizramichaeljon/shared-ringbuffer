
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

uint32_t prompt_uint32(const std::string& prompt, uint32_t default_val) {
    std::string input;
    std::cout << prompt << " [" << default_val << "]: ";
    std::getline(std::cin, input);
    if (input.empty()) return default_val;
    return static_cast<uint32_t>(std::stoul(input));
}

float prompt_float(const std::string& prompt, float default_val) {
    std::string input;
    std::cout << prompt << " [" << default_val << "]: ";
    std::getline(std::cin, input);
    if (input.empty()) return default_val;
    return std::stof(input);
}

int main(int argc, char* argv[]) {
    uint32_t sampleRate = 48000;
    float duration = 2.0f;
    uint32_t channels = 2;
    shmName = "ringbuffer_audio";

    bool interactive = (argc == 1);

    if (!interactive) {
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
        sampleRate = result["sample-rate"].as<uint32_t>();
        duration = result["duration"].as<float>();
        channels = result["channels"].as<uint32_t>();
    } else {
        std::cout << "sharedBuffer setup (interactive mode)\n" << std::endl;
        sampleRate = prompt_uint32("Enter sample rate", sampleRate);
        duration = prompt_float("Enter duration in seconds", duration);
        channels = prompt_uint32("Enter number of channels", channels);
        std::cout << "Using shared memory name: " << shmName << std::endl;
    }

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

        std::cout << "\n[sharedBuffer] Shared memory created: " << shmName << std::endl;
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
