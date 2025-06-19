#pragma once
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
#include <iostream>

namespace bip = boost::interprocess;

class SharedRingBuffer {
public:
    SharedRingBuffer(const std::string& name, size_t size)
        : shm_name(name), buffer_size(size) {

        // Remove any existing shared memory segment with the same name
        bip::shared_memory_object::remove(shm_name.c_str());

        // Create shared memory segment
        shm = bip::shared_memory_object(bip::create_only, shm_name.c_str(), bip::read_write);

        // Set the size of the shared memory segment
        shm.truncate(buffer_size);

        // Map the shared memory segment into this process's address space
        region = bip::mapped_region(shm, bip::read_write);

        // Get a pointer to the memory buffer
        data = static_cast<char*>(region.get_address());

        std::cout << "[SharedRingBuffer] Created shared memory segment of size " << buffer_size << " bytes." << std::endl;
    }

    void writeDummyData() {
        for (size_t i = 0; i < buffer_size; ++i)
            data[i] = static_cast<char>(i % 256);
        std::cout << "[SharedRingBuffer] Dummy data written to shared memory." << std::endl;
    }

private:
    std::string shm_name;
    size_t buffer_size;
    bip::shared_memory_object shm;
    bip::mapped_region region;
    char* data;
};