#include "SharedRingBufferReader.hpp"
#include <iostream>

int main() {
    SharedRingBufferReader reader("MySharedBuffer");
    reader.printData();
    std::cout << "[reader] Data read complete." << std::endl;
    return 0;
}
