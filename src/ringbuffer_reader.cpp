#include "../include/SharedRingBufferReader.hpp"
#include <iostream>

int main() {
    SharedRingBufferReader reader("ringbuffer_audio");
    constexpr int N = 1024;

    std::vector<float> samples = reader.getLatestSamples(N);

    std::cout << "[ringbuffer_reader] Latest " << N << " samples:\n";
    for (float s : samples) std::cout << s << "\n";

    return 0;
}