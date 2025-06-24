#include "../include/SharedRingBufferReader.hpp"
#include <fstream>
#include <chrono>
#include <thread>
#include <iostream>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    SharedRingBufferReader reader("ringbuffer_audio");  // now uses hardcoded name internally
    constexpr int sampleRate = 48000;
    constexpr int N = sampleRate;

    for (int i = 0; i < 3; ++i) {
        std::vector<float> out = reader.getLatestSamples(N);

        std::ofstream csv("out_" + std::to_string(i) + ".csv");
        for (float f : out) csv << f << "\n";

        std::vector<int> x(N);
        for (int j = 0; j < N; ++j) x[j] = j;

        plt::figure_size(1200, 400);
        plt::plot(x, out);
        plt::title("Audio Buffer Snapshot " + std::to_string(i));
        plt::xlabel("Sample Index");
        plt::ylabel("Amplitude");
        plt::save("waveform_" + std::to_string(i) + ".png");

        std::cout << "[Test] Exported CSV and waveform_" << i << ".png\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}