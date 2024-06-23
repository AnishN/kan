SSE: 8x 128-bit regs (xmm0 - xmm7)
* Extended w/ xmm8 - xmm15 (in x86_64)
SSE just used 1 type: 4x f32
SSE2 expanded xmm to repr: 2x f64, 2x i64, 4x i32, 8x i16, 16x i8

Name	Extension sets	Registers	Types
Legacy SSE	SSE–SSE4.2	xmm0–xmm15	single floats. From SSE2: bytes, words, doublewords, quadwords and double floats.
AVX-128 (VEX)	AVX, AVX2	xmm0–xmm15	bytes, words, doublewords, quadwords, single floats and double floats.
AVX-256 (VEX)	AVX, AVX2	ymm0–ymm15	single float and double float. From AVX2: bytes, words, doublewords, quadwords.
AVX-128 (EVEX)	AVX-512VL	xmm0–xmm31 (k0–k7)	doublewords, quadwords, single float and double float. With AVX512BW: bytes and words. With AVX512-FP16: half float.
AVX-256 (EVEX)	AVX-512VL	ymm0–ymm31 (k0–k7)	doublewords, quadwords, single float and double float. With AVX512BW: bytes and words. With AVX512-FP16: half float.
AVX-512 (EVEX)	AVX-512F	zmm0–zmm31 (k0–k7)	doublewords, quadwords, single float and double float. With AVX512BW: bytes and words. With AVX512-FP16: half float.

SSE instrs (per wiki):
Floating-point instructions
Memory-to-register/register-to-memory/register-to-register data movement
Scalar – MOVSS
Packed – MOVAPS, MOVUPS, MOVLPS, MOVHPS, MOVLHPS, MOVHLPS, MOVMSKPS
Arithmetic
Scalar – ADDSS, SUBSS, MULSS, DIVSS, RCPSS, SQRTSS, MAXSS, MINSS, RSQRTSS
Packed – ADDPS, SUBPS, MULPS, DIVPS, RCPPS, SQRTPS, MAXPS, MINPS, RSQRTPS
Compare
Scalar – CMPSS, COMISS, UCOMISS
Packed – CMPPS
Data shuffle and unpacking
Packed – SHUFPS, UNPCKHPS, UNPCKLPS
Data-type conversion
Scalar – CVTSI2SS, CVTSS2SI, CVTTSS2SI
Packed – CVTPI2PS, CVTPS2PI, CVTTPS2PI
Bitwise logical operations
Packed – ANDPS, ORPS, XORPS, ANDNPS
Integer instructions
Arithmetic
PMULHUW, PSADBW, PAVGB, PAVGW, PMAXUB, PMINUB, PMAXSW, PMINSW
Data movement
PEXTRW, PINSRW
Other
PMOVMSKB, PSHUFW
Other instructions
MXCSR management
LDMXCSR, STMXCSR
Cache and Memory management
MOVNTQ, MOVNTPS, MASKMOVQ, PREFETCH0, PREFETCH1, PREFETCH2, PREFETCHNTA, SFENCE

si256 – signed 256-bit integer
si128 – signed 128-bit integer
epi8, epi32, epi64 — an vector of signed 8-bit integers (32 in a __m256 and 16 in a __m128) or signed 32-bit integers or signed 64-bit integers
* Can be slices afaict
epu8 — an vecotr of unsigned 8-bit integers (when there is a difference between what an operation would do with signed and unsigned numbers, such as with conversion to a larger integer or multiplication)
epu16, epu32 — an array of unsigned 16-bit integers or 8 unsigned 32-bit integers (when the operation would be different than signed)
ps — “packed single” — 8 single-precision floats
pd — “packed double” — 4 doubles
ss — one float (only 32-bits of a 256-bit or 128-bit value are used)
sd — one double (only 64-bits of a 256-bit or 256-bit value are used)

mmx (1997): 64-bit (int) -> 2x i32
sse (1999): 128-bit (float and int) -> 4x f32
* sse2 (2001)
* sse3 (2004)
* sse4 (2006)
avx (2008): 256-bit (float) -> 8x f32
* fma3 (2013): fused mul-add (a * b + c)
    * the 3 means for 3 operands
    * output is stored at 1 of the inputs (a, b, or c)
* avx2 (2013): (int) -> 8x i32
avx-512 (2016): 512-bit (float and int) -> 16 x f32
    * messy and sub-typed

how to simd:
1. use -O3 and pray
2. unwrap loops (e.g. by factor of 4 or 8) and then pray (see #1)
3. intrinsic functions (almost map to specific asm instructions)
    * use a specific vec instruction, compiler still has to make it work
    * gets most of the perf gains
4. asm language (do all the work)
    * maps to binary machine code direct

diff b/t 3 and 4:
asm: 
* vaddps ymm2, ymm1, ymm0
* add a set of "packed" singles (aka 8x f32)
* reg inputs are 2 and 1, reg output is 0
intrinsic:
* c = _mm256_add_ps(b, a)

headers:
* mmintrin.h -> mmx
* xmmintrin.h -> sse
* emmintrin.h -> sse2
* pmmintrin.h -> sse3
* tmmintrin.h -> ssse3 (supplemental sse3 - added 16 instrs)
* smmintrin.h -> sse4.1
* nmmintrin.h -> sse4.2
* ammintrin.h -> sse4a
* wmmintrin.h -> aes
* immintrin.h -> avx, avx2, fma, avx-512
    * jut include this last one for all compilers and all instr sets :)

intrinsics data types:
all packed (float, int, double) blocks of data
* __m64
    * mmx -> not using in modern code
    * repr: 8x 8bit, 4x 16bit, 2x 32bit, 1x 64bit
* __m128, __m128i, __m128d
    * sse(1-4) -> not uncommon to use these
    * all packed
    * __m128: 4x f32
    * __m128i: 2x 64, 4x i32, 8x i16, 16x i8
    * __m128d: 2x f64
    * can be signed or unsigned int (just funcs differ)
* __m256, __m256i, __m256d
    * avx and avx2 (2008+)
    * all x86 cpus after 2012 support them
    * __m256: 8x f32
    * __m256i: 4x i64, 8x 32, 16x i16, 32x i8
    * __m256d: 4x f64
* __m512, __m512i, __m512d

mem should be aligned
* now, perf penalty is less for unaligned than historically
* there are aligned vs unaligned instructions
* can align to 32 bytes (or 64 bytes for cache line purposes)
* packed singles (ps) = packed f32
* packed doubles (pd) = packed f64
* extended packed integers (epi) = packed ints lol
* extended packed uints (epu) = packed uints
    * not too many unsigned funcs (since 2's complement ops be the same)

200 avx1 functions
general groups:
* base_math (add, sub, mul, etc.)
* logical ops (and, or, xor, etc.)
* load/store (r/w from main mem)
* cmp (gt, lt, etc.)
* permute/blend/insert-extract (shuffle elem order, cut/paste parts, etc.)
* broadcast/scatter/gather (distrib/collect many elems from a single block)
* math func (sqrt, max, ceil, etc.)
* conv (f32 -> i32, f32 -> f64, f64 -> i32)
* init (set desired values like all 0)
* cast (reinterpret data as float instead of int)

ref: https://db.in.tum.de/~finis/x86-intrin-cheatsheet-v2.1.pdf
* Very useful cheatsheet grouping (broader groups)
ref: https://www.laruence.com/sse/#
* Useful intrinsics func list (by gen/ also w/ groups)
ref: https://chryswoods.com/vector_c++/README.html
* Short vectorization course w/ intrinsics
ref: https://www.cs.virginia.edu/~cr4bd/3330/F2018/simdref.html
* Basic SIMD ref fr CS 3300 course @ Virginia
ref: https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
* Intel intrinsics guid (like laruence version!)
ref: https://stackoverflow.com/questions/31490853/are-different-mmx-sse-and-avx-versions-complementary-or-supersets-of-each-other
* Nice overview of generations of instruction sets (and intrinsics)
ref: https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions
* Good overview of SSE data types and instructions
ref: https://www.youtube.com/watch?v=AT5nuQQO96o
* Fantastic SIMD (AVX) instr overview