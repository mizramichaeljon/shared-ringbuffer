
#include <boost/interprocess/shared_memory_object.hpp>
#include <iostream>

int main() {
    const char* shmName = "ringbuffer_audio";
    bool removed = boost::interprocess::shared_memory_object::remove(shmName);
    if (removed) {
        std::cout << "[Cleanup] Successfully removed shared memory: " << shmName << std::endl;
    } else {
        std::cout << "[Cleanup] Shared memory not found or already removed: " << shmName << std::endl;
    }

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
