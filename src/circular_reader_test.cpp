#include "../include/SharedRingBufferReader.hpp"
#include <fstream>
#include <chrono>
#include <thread>
#include <iostream>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main()
{
    std::cout << "[Main] Entered reader test main()\n";
    SharedRingBufferReader reader("ringbuffer_audio");

    constexpr int sampleRate = 48000;
    constexpr int N = sampleRate;

    for (int i = 0; i < 3; ++i)
    {
        std::vector<float> out = reader.getLatestSamples(N);

        if (out.empty())
        {
            std::cerr << "[Reader] WARNING: Received empty sample buffer\n";
        }
        else
        {
            std::cout << "[Reader] First sample: " << out.front()
                      << ", Last sample: " << out.back() << "\n";
        }

        // Save CSV for inspection
        std::ofstream csv("out_" + std::to_string(i) + ".csv");
        for (float f : out)
            csv << f << "\n";

        try
        {
            std::vector<int> x;
            std::vector<float> y;

            // Only take the first 1000 samples for clarity
            int visibleSamples = std::min(N, 1000);
            x.reserve(visibleSamples);
            y.reserve(visibleSamples);

            for (int j = 0; j < visibleSamples; ++j)
            {
                x.push_back(j);
                y.push_back(out[j]);
            }

            plt::figure_size(1000, 400);
            plt::plot(x, y);
            plt::title("Audio Buffer Snapshot " + std::to_string(i));
            plt::xlabel("Sample Index");
            plt::ylabel("Amplitude");
            plt::ylim(-1.1, 1.1); // enforce full sine wave range
            plt::grid(true);
            plt::tight_layout();
            plt::save("clean_waveform_" + std::to_string(i) + ".png");
            std::cout << "[Plot] Saved clean_waveform_" << i << ".png\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << "[Plotting Error] Failed to create plot " << i
                      << ": " << e.what() << "\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}