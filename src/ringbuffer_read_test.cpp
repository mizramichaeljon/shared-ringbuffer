#include "../include/SharedRingBufferReader.hpp"
#include <thread>
#include <chrono>

int main() {
    const std::string shmName = "ringbuffer_audio";

    std::cout << "[Reader] Waiting for shared memory '" << shmName << "'..." << std::endl;

    // Loop until the buffer becomes available
    while (true) {
        try {
            SharedRingBufferReader reader(shmName);
            reader.printData();  // Print first few bytes
            break;
        } catch (const std::exception& e) {
            std::cout << "[Reader] Waiting for shared memory '" << shmName << "'..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}