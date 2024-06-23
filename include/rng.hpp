#pragma once
#include "num.hpp"

/*
Non-cryptographic pseudorandom number generator (PRNG).

Code adapted from Bob Jenkins's Small Fast (JSF) PRNG (public domain). 
Original version here:
https://burtleburtle.net/bob/rand/smallprng.html

8 and 16 bit versions adapted from this analysis here:
https://www.pcg-random.org/posts/bob-jenkins-small-prng-passes-practrand.html
*/

// Uses a single 64-bit number to seed all 4 sizes of PRNG (8, 16, 32, 64 bit)

namespace rng {
    void init(uint64_t seed);

    bool gen_bool();

    uint8_t gen_uint8();
    uint16_t gen_uint16();
    uint32_t gen_uint32();
    uint64_t gen_uint64();
    int8_t gen_int8();
    int16_t gen_int16();
    int32_t gen_int32();
    int64_t gen_int64();
    float32_t gen_float32();
    float64_t gen_float64();

    uint8_t gen_range_uint8(uint8_t low, uint8_t high);
    uint16_t gen_range_uint16(uint16_t low, uint16_t high);
    uint32_t gen_range_uint32(uint32_t low, uint32_t high);
    uint64_t gen_range_uint64(uint64_t low, uint64_t high);
    int8_t gen_range_int8(int8_t low, int8_t high);
    int16_t gen_range_int16(int16_t low, int16_t high);
    int32_t gen_range_int32(int32_t low, int32_t high);
    int64_t gen_range_int64(int64_t low, int64_t high);
    float32_t gen_range_float32(float32_t low, float32_t high);
    float64_t gen_range_float64(float32_t low, float32_t high);
};