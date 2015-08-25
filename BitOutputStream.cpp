#include "BitOutputStream.hpp"

using namespace std;

void BitOutputStream::writeBit(int bit) {
    if (bufi == 8)
        flush();

    // Writes next bit to buffer
    bit = bit << bufi++;
    buf = buf | bit;
}

void BitOutputStream::writeByte(int b) {
    // Writes the least significant bit to stream
    b = b & 0xFF;
    out.put(b);
}

// void BitOutputStream::writeInt(int i) {

// }

void BitOutputStream::flush() {
    out.put(buf);
    out.flush();
    buf = bufi = 0;
}
