#pragma once
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <string>

namespace bip = boost::interprocess;

class SharedRingBufferReader {
public:
    SharedRingBufferReader(const std::string& name)
        : shm_name(name) {
        shm = bip::shared_memory_object(bip::open_only, shm_name.c_str(), bip::read_only);
        region = bip::mapped_region(shm, bip::read_only);
        data = static_cast<const char*>(region.get_address());
        buffer_size = region.get_size();

        std::cout << "[SharedRingBufferReader] Connected to shared memory: " << buffer_size << " bytes." << std::endl;
    }

    void printData() {
        for (size_t i = 0; i < buffer_size && i < 64; ++i) { // Print only first 64 bytes
            std::cout << (int)(unsigned char)data[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    std::string shm_name;
    bip::shared_memory_object shm;
    bip::mapped_region region;
    const char* data;
    size_t buffer_size;
};
