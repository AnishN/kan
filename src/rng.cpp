#include "rng.hpp"

// Private types
typedef struct Rand8Ctx { 
    uint8_t a; 
    uint8_t b; 
    uint8_t c; 
    uint8_t d; 
} Rand8Ctx;

typedef struct Rand16Ctx { 
    uint16_t a; 
    uint16_t b; 
    uint16_t c; 
    uint16_t d; 
} Rand16Ctx;

typedef struct Rand32Ctx { 
    uint32_t a; 
    uint32_t b; 
    uint32_t c; 
    uint32_t d; 
} Rand32Ctx;

typedef struct Rand64Ctx { 
    uint64_t a; 
    uint64_t b; 
    uint64_t c; 
    uint64_t d; 
} Rand64Ctx;

// Private data
static Rand8Ctx rng8_ctx;
static Rand16Ctx rng16_ctx;
static Rand32Ctx rng32_ctx;
static Rand64Ctx rng64_ctx;

// Private macros
#define ROT_8(x, k) (((x)<<(k))|((x)>>(8-(k))))
#define ROT_16(x, k) (((x)<<(k))|((x)>>(16-(k))))
#define ROT_32(x, k) (((x)<<(k))|((x)>>(32-(k))))
#define ROT_64(x, k) (((x)<<(k))|((x)>>(64-(k))))

// Private functions

/*
Converts the uint32_t value into a float32_t in range [0.0, 1.0).
Exploits the bit storage used for the IEEE-754 format.
Internally uses evil union type punning.
layout of float32_t (little endian):
* frac(tional) = 23 bits
* exp(onent) = 8 bits (offset by 127)
* sign = 1 bit
Shift right to remove the exp/sign data.
Then OR with exp/sign data that has a sign and exponent of zero
The result is that frac will be multiplied by 2^0 (aka 1).
The generated range would be [1.0, 2.0), hence the - 1.0 at the end.
*/
float32_t uint32_to_float32(uint32_t val) {
    union { 
        uint32_t uint32; 
        float32_t float32; 
    } conv = { .uint32 = val >> 9 | ((uint32_t)127 << 23) };
    return conv.float32 - 1.0;
}

/*
Converts the uint64_t value into a float64_t in range [0.0, 1.0).
Exploits the bit storage used for the IEEE-754 format.
Internally uses evil union type punning.
layout of float64_t (little endian):
* frac(tional) = 52 bits
* exp(onent) = 11 bits (offset by 1023)
* sign = 1 bit
Shift right to remove the exp/sign data.
Then OR with exp/sign data that has a sign and exponent of zero
The result is that frac will be multiplied by 2^0 (aka 1).
The generated range would be [1.0, 2.0), hence the - 1.0 at the end.
*/
float64_t uint64_to_float64(uint64_t val) {
    union { 
        uint64_t uint64; 
        float64_t float64; 
    } conv = { .uint64 = val >> 12 | ((uint64_t)1023 << 52) };
    return conv.float64 - 1.0;
}

uint8_t rng8(Rand8Ctx *x) {
    uint8_t e = x->a - ROT_8(x->b, 1);
    x->a = x->b ^ ROT_8(x->c, 4);
    x->b = x->c + x->d;
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}

void rng8_init(Rand8Ctx *x, uint8_t seed) {
    x->a = 0xed, x->b = x->c = x->d = seed;
    for (uint8_t i=0; i<20; ++i) {
        (void)rng8(x);
    }
}

uint16_t rng16(Rand16Ctx *x) {
    uint16_t e = x->a - ROT_16(x->b, 13);
    x->a = x->b ^ ROT_16(x->c, 8);
    x->b = x->c + x->d;
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}

void rng16_init(Rand16Ctx *x, uint16_t seed) {
    x->a = 0x5eed, x->b = x->c = x->d = seed;
    for (uint16_t i=0; i<20; ++i) {
        (void)rng16(x);
    }
}

uint32_t rng32(Rand32Ctx *x) {
    uint32_t e = x->a - ROT_32(x->b, 27);
    x->a = x->b ^ ROT_32(x->c, 17);
    x->b = x->c + x->d;
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}

void rng32_init(Rand32Ctx *x, uint32_t seed) {
    uint32_t i;
    x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
    for (i=0; i<20; ++i) {
        (void)rng32(x);
    }
}

uint64_t rng64(Rand64Ctx *x) {
    uint64_t e = x->a - ROT_64(x->b, 39);
    x->a = x->b ^ ROT_64(x->c, 11);
    x->b = x->c + x->d;
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}

void rng64_init(Rand64Ctx *x, uint64_t seed) {
    uint64_t i;
    x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
    for (i=0; i<20; ++i) {
        (void)rng64(x);
    }
}


void rng::init(uint64_t seed) {
    rng64_init(&rng64_ctx, seed);
    rng32_init(&rng32_ctx, seed);
    rng16_init(&rng16_ctx, seed);
    rng8_init(&rng8_ctx, seed);
}

bool rng::gen_bool() {
    return rng::gen_uint8() & 1;
}

uint8_t rng::gen_uint8() { 
    return rng8(&rng8_ctx);
}

uint16_t rng::gen_uint16() {
    return rng16(&rng16_ctx);
}

uint32_t rng::gen_uint32() {
    return rng32(&rng32_ctx);
}

uint64_t rng::gen_uint64() {
    return rng64(&rng64_ctx);
}

int8_t rng::gen_int8() {
    return (int8_t)rng::gen_uint8();
}

int16_t rng::gen_int16() {
    return (int16_t)rng::gen_uint16();
}

int32_t rng::gen_int32() {
    return (int32_t)rng::gen_uint32();
}

int64_t rng::gen_int64() {
    return (int64_t)rng::gen_uint64();
}

float32_t rng::gen_float32() {
    return uint32_to_float32(rng::gen_uint32());
}

float64_t rng::gen_float64() {
    return uint64_to_float64(rng::gen_uint64());
}

uint8_t rng::gen_range_uint8(uint8_t low, uint8_t high) {
    return rng::gen_uint8() % (high - low) + low;
}

uint16_t rng::gen_range_uint16(uint16_t low, uint16_t high) {
    return rng::gen_uint16() % (high - low) + low;
}

uint32_t rng::gen_range_uint32(uint32_t low, uint32_t high) {
    return rng::gen_uint32() % (high - low) + low;
}

uint64_t rng::gen_range_uint64(uint64_t low, uint64_t high) {
    return rng::gen_uint64() % (high - low) + low;
}

int8_t rng::gen_range_int8(int8_t low, int8_t high) {
    return rng::gen_int8() % (high - low) + low;
}

int16_t rng::gen_range_int16(int16_t low, int16_t high) {
    return rng::gen_int16() % (high - low) + low;
}

int32_t rng::gen_range_int32(int32_t low, int32_t high) {
    return rng::gen_int32() % (high - low) + low;
}

int64_t rng::gen_range_int64(int64_t low, int64_t high) {
    return rng::gen_int64() % (high - low) + low;
}

float32_t rng::gen_range_float32(float32_t low, float32_t high) {
    return rng::gen_float32() * (high - low) + low;
}

float64_t rng::gen_range_float64(float32_t low, float32_t high) {
    return rng::gen_float64() * (high - low) + low;
}