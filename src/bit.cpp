#include "bit.hpp"

uint8_t bit_next_pow2_uint8(uint8_t n) {
    uint8_t out = n;
    out--;
    out |= out >> 1;
    out |= out >> 2;
    out |= out >> 4;
    out++;
    return out;
}

uint16_t bit_next_pow2_uint16(uint16_t n) {
    uint16_t out = n;
    out--;
    out |= out >> 1;
    out |= out >> 2;
    out |= out >> 4;
    out |= out >> 8;
    out++;
    return out;
}

uint32_t bit_next_pow2_uint32(uint32_t n) {
    uint32_t out = n;
    out--;
    out |= out >> 1;
    out |= out >> 2;
    out |= out >> 4;
    out |= out >> 8;
    out |= out >> 16;
    out++;
    return out;
}

uint64_t bit_next_pow2_uint64(uint64_t n) {
    uint64_t out = n;
    out--;
    out |= out >> 1;
    out |= out >> 2;
    out |= out >> 4;
    out |= out >> 8;
    out |= out >> 16;
    out |= out >> 32;
    out++;
    return out;
}