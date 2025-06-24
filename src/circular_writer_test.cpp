#include "../include/SharedRingBufferWriter.hpp"
#include <cmath>
#include <thread>

int main() {
    constexpr int sampleRate = 48000;
    constexpr float freq = 440.0f;
    constexpr int bufferLength = sampleRate;
    constexpr int blockSize = 512;

    SharedRingBuffer ring("ringbuffer_audio", bufferLength * sizeof(float));

    float phase = 0.0f;
    float twoPiF = 2 * M_PI * freq / sampleRate;

    while (true) {
        float buf[blockSize];
        for (int i = 0; i < blockSize; ++i) {
            buf[i] = std::sin(phase);
            phase += twoPiF;
            if (phase >= 2 * M_PI) phase -= 2 * M_PI;
        }
        ring.write(buf, blockSize);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}