#pragma once
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
#include <iostream>
#include <cstring>

namespace bip = boost::interprocess;

class SharedRingBuffer {
public:
    SharedRingBuffer(const std::string& name, size_t size)
        : shm_name(name), buffer_size(size) {
        bip::shared_memory_object::remove(shm_name.c_str());
        shm = bip::shared_memory_object(bip::create_only, shm_name.c_str(), bip::read_write);
        shm.truncate(buffer_size);
        region = bip::mapped_region(shm, bip::read_write);
        data = static_cast<char*>(region.get_address());
        std::cout << "[SharedRingBuffer] Created shared memory (" << shm_name
                  << ") size " << buffer_size << " bytes." << std::endl;
    }

    void write(const float* src, size_t numSamples) {
        size_t bytes = numSamples * sizeof(float);
        if (bytes > buffer_size) {
            std::cerr << "[SharedRingBuffer] Warning: truncating write from "
                      << bytes << " to " << buffer_size << " bytes." << std::endl;
            bytes = buffer_size;
        }
        std::memcpy(data, src, bytes);
    }

    // Stub to satisfy demo build (no-op)
    void writeDummyData() {
        std::cout << "[SharedRingBuffer] writeDummyData() is deprecated and now a no-op.\n";
    }

    void close() {
        bip::shared_memory_object::remove(shm_name.c_str());
        std::cout << "[SharedRingBuffer] Closed shared memory (" << shm_name << ")." << std::endl;
    }

private:
    std::string shm_name;
    size_t buffer_size;
    bip::shared_memory_object shm;
    bip::mapped_region region;
    char* data;
};