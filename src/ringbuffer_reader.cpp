#include "SharedRingBufferReader.hpp"

int main() {
    SharedRingBufferReader reader("ringbuffer_audio");
    reader.printData();
    return 0;
}