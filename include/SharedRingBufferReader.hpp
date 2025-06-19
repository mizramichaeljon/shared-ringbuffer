#pragma once
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

namespace bip = boost::interprocess;

class SharedRingBufferReader {
public:
    SharedRingBufferReader(const std::string& name)
        : shm_name(name) {
        while (true) {
            try {
                shm = bip::shared_memory_object(bip::open_only, shm_name.c_str(), bip::read_only);
                break;
            } catch (const bip::interprocess_exception&) {
                std::cout << "[Reader] Waiting for shared memory '" << shm_name << "'...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }

        region = bip::mapped_region(shm, bip::read_only);
        data = static_cast<const char*>(region.get_address());
        buffer_size = region.get_size();

        std::cout << "[SharedRingBufferReader] Connected to shared memory: " 
                  << buffer_size << " bytes." << std::endl;
    }

    void printData() const {
        for (size_t i = 0; i < buffer_size && i < 64; ++i) {
            std::cout << (int)(unsigned char)data[i] << " ";
        }
        std::cout << std::endl;
    }

    // ✅ Accessors for external use
    const void* rawData() const { return static_cast<const void*>(data); }
    size_t bufferSize() const { return buffer_size; }

private:
    std::string shm_name;
    bip::shared_memory_object shm;
    bip::mapped_region region;
    const char* data;
    size_t buffer_size;
};