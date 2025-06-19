// ringbuffer_to_wav.cpp
// Reads from shared memory and writes to a .wav file

#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdint>
#include "SharedRingBufferReader.hpp"

// Write a very basic 16-bit mono WAV file header
void writeWavHeader(std::ofstream& outFile, uint32_t numSamples, uint32_t sampleRate) {
    uint32_t byteRate = sampleRate * sizeof(int16_t);
    uint32_t blockAlign = sizeof(int16_t);
    uint32_t dataChunkSize = numSamples * sizeof(int16_t);
    uint32_t fmtChunkSize = 16;
    uint32_t fileSize = 4 + (8 + fmtChunkSize) + (8 + dataChunkSize);

    // RIFF chunk descriptor
    outFile.write("RIFF", 4);
    outFile.write(reinterpret_cast<const char*>(&fileSize), 4);
    outFile.write("WAVE", 4);

    // fmt subchunk
    outFile.write("fmt ", 4);
    outFile.write(reinterpret_cast<const char*>(&fmtChunkSize), 4);
    uint16_t audioFormat = 1; // PCM
    uint16_t numChannels = 1; // mono
    outFile.write(reinterpret_cast<const char*>(&audioFormat), 2);
    outFile.write(reinterpret_cast<const char*>(&numChannels), 2);
    outFile.write(reinterpret_cast<const char*>(&sampleRate), 4);
    outFile.write(reinterpret_cast<const char*>(&byteRate), 4);
    outFile.write(reinterpret_cast<const char*>(&blockAlign), 2);
    uint16_t bitsPerSample = 16;
    outFile.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // data subchunk
    outFile.write("data", 4);
    outFile.write(reinterpret_cast<const char*>(&dataChunkSize), 4);
}

int main() {
    const std::string shmName = "ringbuffer_audio";
    const std::string outFilePath = "output.wav";
    const uint32_t sampleRate = 48000;

    SharedRingBufferReader reader(shmName);
    size_t numBytes = reader.getBufferSize();
    size_t numSamples = numBytes / sizeof(float);

    // Convert float [-1.0, 1.0] to int16_t [-32768, 32767]
    const float* floatData = reinterpret_cast<const float*>(reader.getData());
    std::vector<int16_t> int16Data(numSamples);
    for (size_t i = 0; i < numSamples; ++i) {
        float sample = floatData[i];
        sample = std::max(-1.0f, std::min(1.0f, sample)); // clamp
        int16Data[i] = static_cast<int16_t>(sample * 32767);
    }

    std::ofstream outFile(outFilePath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output.wav for writing.\n";
        return 1;
    }

    writeWavHeader(outFile, static_cast<uint32_t>(numSamples), sampleRate);
    outFile.write(reinterpret_cast<const char*>(int16Data.data()), int16Data.size() * sizeof(int16_t));
    outFile.close();

    std::cout << "[WAV Writer] Wrote " << numSamples << " samples to " << outFilePath << "\n";
    return 0;
}