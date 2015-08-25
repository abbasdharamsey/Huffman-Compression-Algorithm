#include "BitInputStream.hpp"

using namespace std;

int BitInputStream::readBit() {
    // Buffer is empty when index is 8
    if (bufi == 8) {
        int readByteRet = readByte();

        if (readByteRet == -1) return -1;
        buf = readByteRet;
    }

    // Shift desired bit to least significant position and return it
    int bitShiftedBuf = buf >> bufi++;
    return bitShiftedBuf & 1;
}

int BitInputStream::readByte() {
    if (in.peek() == EOF) 
        return -1;

    // Reads next byte from stream
    bufi = 0;
    return in.get();
}
