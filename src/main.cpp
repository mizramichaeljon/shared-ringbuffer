#include "SharedRingBuffer.hpp"
#include <iostream>

int main() {
    SharedRingBuffer buffer("MySharedBuffer", 1024);
    buffer.writeDummyData();
    std::cout << "[main] Program complete." << std::endl;
    return 0;
}
