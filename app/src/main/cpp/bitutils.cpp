#include "types.h"

using namespace types;

const _ui bitPositions[] = {
        0,  1, 48,  2, 57, 49, 28,  3,
        61, 58, 50, 42, 38, 29, 17,  4,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 18, 12,  5,
        63, 47, 56, 27, 60, 41, 37, 16,
        54, 35, 52, 21, 44, 32, 23, 11,
        46, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19,  9, 13,  8,  7,  6
};

_ui getBit(_ci &number, _ci position) {
    return (number >> position) & 1;
}

_ui setBit(_ci &number, _ci position) {
    return number | (1 << position);
}

_ui getLowestBit(_ci number) {
    return bitPositions[((number & -number) * 0x03F79D71B4CB0A89ULL) >> 58];
}

_ui powerLowestBit(_ci number) {
    return number & (-number);
}

_ui zeroLowestBit(_ci number) {
    return number & (number - 1);
}

void zeroLowestBitAssign(_ui &number) {
    number &= number - 1;
}

_ui bitCount(_ui number) {
    number -= (number >> 1) & 1431655765;
    number = (number & 858993459) + ((number >> 2) & 858993459);
    number = number + (number >> 4) & 252645135;
    number += number >> 8;
    number += number >> 16;
    return number & 63;
}