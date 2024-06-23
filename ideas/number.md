I want all of the types to be capitalized.
We use the following sized types:

* int8_t, int16_t, int32_t, int64_t
* Uint8_t, Uint16_t, Uint32_t, Uint64_t
* Float16, float32_t, float64_t
* Later: Float80, BFloat16
    * Float80 is for useless MMX stuff
    * BFloat16 is for Google/neural network "Brain Float 16" format

What about size types?
ISize, USize
IAddr, UAddr
IDiff, UDiff

Swift's SIMD type naming is meh:
struct SIMD2: A vector of two scalar values.
struct SIMD3: A vector of three scalar values.
struct SIMD4: A vector of four scalar values.
struct SIMD8: A vector of eight scalar values.
struct SIMD16: A vector of 16 scalar values.
struct SIMD32: A vector of 32 scalar values.
struct SIMD64: A vector of 64 scalar values.

There are other platform-specific types to consider, not just usize/isize.
Certainly for flat memory models, the following c relations hold:
* 32-bit: sizeof(uint64_t) = sizeof(u/intptr_t) = sizeof(ptrdiff_t) = 4
* 64-bit: sizeof(uint64_t) = sizeof(u/intptr_t) = sizeof(ptrdiff_t) = 8
But, this is *not* always true, such as in these cases:
* Old systems: 16-bit segmented memory models
    * Addresses had 2 parts:
        * Segment: 'S' registers
            * CS = code
            * DS = data
            * ES = extra
            * SS = stack
        * Offset
    * Code (c) ptr: near and far
    * Data (d) ptr: near and far
    * far ptrs were needed when exceeded 64 kB of c or d...
    * cd combos: 
        * near-near = small
        * near-far = compact
        * far-near = medium
        * far-far = large
    * also MS-DOS had tiny: code + data in single 64 kb segment
* New systems: CHERI
    * Addresses both **spatial safety** and **temporal safety** (use after free)
    * Guarantees ptr integrity and provenance (efficient dyn bounds-checking)
    * Uses a new `Capability` type (a bounds-checked pointer w/ integrity)
        * Held in memory in extended int registers
        * 64 bits (address) + another 64 bits (metadata)
            * Metadata include:
                * permissions (r/w)
                * (compressed) bounds (range of valid virt mem)
                    * base + len
                * special bit
            * Also a *hidden validity tag* (stored in a secret 129th bit)
        * Adds special loads and stores instr w/ bounds-check cap
            * throws HW exception if address is OOB
        * Guarded manipulations: esp decrease bounds + decrease permissions
            * Formally verified in HW
            * You can only shrink and never grow caps
    * Sealed mode: need to unseal the ptr w/ compartmnt transioin
        * Adds 24 bit obj type, and compress bounds further
    * Started w/ sealed code cap + sealed data cap
    * Have compartmental models
    * Like machine-level TLB + page-table walkers
    * Have multiple modes (legacy <-> modern):
        * Unmodified - all ptrs are ints
        * Hybrid - annotated and auto-selected ptrs are caps
        * Pure-cap - all ptrs are caps

But there are semantic differences:
* intptr_t/uintptr_t (aka iptr/uptr): able to fit C's void *
* usize and isize are confusing:
    * Rust has odd definitions: 
        * usize = has same # bits as platform's pointer type; can hold every mem address
            * Community: for memory **addresses**, positions, indices, lengths (or sizes!).
        * isize = has same # bits as platform's pointer type; the max bound on object/array size
            * Community: for **offsets** to addresses, positions, indices, or lengths.
        * This conflates a few things together:
            * usize basically becomes both uintptr_t and uint64_t
            * isize basically becomes both intptr_t and uint64_t
* ptrdiff_t (aka ioff/ioff): for pointer arithmetic
* CHERI recommendations of c int/ptr/ptr-esque types as follows:
    * pure integer types (both with and without specified bit size):
        * e.g.: int, int32_t, long, int64_t
        * **Do** use to hold int values
        * **Do not** cast to ptr (unless it is an array offset to another ptr)
    * vaddr_t: 
        * new CHERI type! 
        * holds virtual addresses
        * **Do** combo w/ prior valid cap to address space
            * use cheri_address_set(c, x) to get that deref-able ptr
        * **Do not** convert to ptr and deref
    * uint64_t, suint64_t: 
        * **Do** use to hold the **lengths** of regions of addr space
    * ptrdiff_t: 
        * **Do** use as diff b/t 2 ptrs pointing to diff parts of one array
            * old ptr + ptrdiff -> new ptr
            * new ptr can only be deref in old ptr's bound
        * **Do not** use for anything else
    * intptr_t/uintptr_t: 
        * hold values that **may** be valid ptrs if cast back to ptr type
        * **Do** use if 1+ of the following are true:
            1. you don't know if var is holding an int or a ptr
            2. it is easier to place ptr in int type (for math)
                * math is in units of bytes (aka treat like char *)
        * **Do not** assign random int to it and transform back to ptr
        * observable range of uintptr_t = range of vaddr_t
        * observable range of intptr_t = range of ptrdiff_t
        * Note: intptr_t/uintptr_t have **increased alignment/storage** needs
            * Makes sense: **they hold the entire ptr, not just addr!**
    * intmax_t/uintmax_t:
        * should repr any valid of any (unsigned) int type
        * **Do not** use these
        * they are only **64-bit**, not 129-bit
        * only repr int *range* of intptr_t/uintptr_t (not metadata/tag bit)
        * May use for convenient printf modifier (%j) since PRI* macros suck
    * max_align_t:
        * type must repr the "greatest fundamental alignment" (including caps)
        * **Do** use **alignof**(max_align_t) in custom allocators:
            * not sizeof(long double) or sizeof(uint64_t)
    * char* (ptrs): 
        * **Do** deref/use as long as derived from a valid source, including:
            * other valid ptr
            * other valid intptr_t (cast)
            * other valid uintptr_t (cast)
        * **Do not** CAST TO/FROM arbitrary ints
    * NOTES: 
        1. `sizeof(uintptr_t) != sizeof(uint64_t)`
            * Makes sense: uint64_t just holds addr space len, not metadata
        2. tags only apply if mem is cap-sized + cap-aligned (must align ptrs)
        3. caps can only come from one other cap (single origin provenance)
    * EXAMPLES:
        // '1' is promoted into cap type (uintptr_t). it has no provenance
        // therefore, 'input_ptr' must provide the provenance
        // c1 and c2 can be validated to carry provenance
        void *c1 = (void *)((uintptr_t)input_ptr + 1);
        void *c2 = (void *)(1 + (uintptr_t)input_ptr);
        // the correct provenance source can't be inferred for c3 (so warning)
        uintptr_t offset = 1;
        void *c3 = (void *)(offset + (uintptr_t)input_ptr);
        // possible fixes:
        uint64_t offset_uint64_t = 1;
        void *c3_good1 = (void *)(offset_uint64_t + (uintptr_t)input_ptr);
        uintptr_t offset_uintptr_t = 1;
        void *c3_good2 = (void *)((uint64_t)offset_uintptr_t + (uintptr_t)input_ptr);
    * You may go temp OOB while iter over array (e.g. inc ptr before the overflow check)
    * CHERI-MIPS 64-bit CHERI-RISCV hav 27 bits for bounds:
        * Ptr can travel forward max(sizeof(obj)/4, 2 KiB)
        * Ptr can travel backward max(sizeof(obj)/8, 2 KiB)
    * Cannot rely on that OOB behavior though
    * ptr cmp ops (==, <, <=) only take addr, not tag/metadata, into account
* Use these for ptr alignment:
    * T __builtin_align_down(T ptr, uint64_t alignment)
    * T __builtin_align_up(T ptr, uint64_t alignment)
    *  _Bool __builtin_is_aligned(T ptr, uint64_t alignment)
    * these 3 funcs are preferred (removes unneeded uintptr_t casts)
* Use these macros to store additional data in pointers:
    * Cannot use high bits, only low bits
        * as those will be zero depending on alignment
    * uintptr_t cheri_low_bits_clear(uintptr_t ptr, vaddr_t mask)
        * clears ptr's low bits (like ptr & ~mask)
        * mask is a bitwise, and should be < _Alignof(ptr)
    * vaddr_t cheri_low_bits_get(uintptr_t ptr, vaddr_t mask)
        * gets ptr's low bits (like ptr & mask)
        * mask is a bitwise, and should be < _Alignof(ptr)
    * uintptr_t cheri_low_bits_or(uintptr_t ptr, vaddr_t bits)
        * bitwise or of ptr w/ bits
        * bt should be < alignment of ptr
    * uintptr_t cheri_low_bits_set(uintptr_t ptr, vaddr_t mask, vaddr_t bits)
        * clears then sets the low bits in mask
* __packed does not work with CHERI (since ptrs are then unaligned)
* While the compiler and native allocators (stack, heap, ...) provide sufficient alignment for capability-based pointers, custom allocators may align allocations to sizeof(intmax_t) rather than alignof(max_align_t).
* CHERI caps are 2x sizeof(integer) (so ptr is 128 bits wide), with 64-bit math addr props. Alignment is 16 bytes on a 64-bit. The 1 bit out-of-band tag is separate.
* Bounds are compressed (exploits redundancy b/t addr and bounds)
    * ptr usually w/in or close to its assoc alloc
    * allocs typically well-aligned
    * compressed w/ float repr (high precision for small objs, but larger alignment/padding for larger alloc)

ptr caps read api:
* vaddr_t cheri_address_get(void *c) // gets addr of c
* vaddr_t cheri_base_get(void *c) // gets lower bound of c
* uint64_t cheri_length_get(void *c) // gets bound length of c (can calc upper)
* uint64_t cheri_offset_get(void *c) // get diff b/t addr and lower bound of c
* uint64_t cheri_perms_get(void *c) // gets perms of c
* _Bool cheri_tag_get(void *c) // gets validity tag of c (note tag not addressable part of mem)

ptr caps write api:
* void *cheri_address_set(void *c, vaddr_t a) // addr -> ptr (if oob, tag bit not set)
* void *cheri_bounds_set(void *c, uint64_t x) // narrow's c bound len (not 100% exact 2/2 cap compression)
* void *cheri_bounds_set_exact(void *c, uint64_t x) // narrow's c bound len exactly (will trap if len is oob)
* void *cheri_perms_and(void *c, uint64_t x) // bit_and of c's perms and val x -> new cap
* void *cheri_tag_clear(void *c)

caps perms:
* CHERI_PERM_EXECUTE // Authorize instruction fetch
* CHERI_PERM_LOAD // Authorize data load
* CHERI_PERM_LOAD_CAP // Authorize capability load. If no perm, clear tag.
* CHERI_PERM_STORE // Authorize data store.
* CHERI_PERM_STORE_CAP // Authorize capability store. If no perm and valid tag, then HW exception.

uint64_t cheri_representable_length(uint64_t len)
* aka what is length w/ hypothetical cheri_bounds_set?
uint64_t cheri_representable_alignment_mask(uint64_t len) 
* get a mask to align addr downwards to make a precisely boundd cap

References:
https://pvs-studio.com/en/blog/posts/cpp/a0050 (uint64_t and ptrdiff_t benefits)
https://en.cppreference.com/w/cpp/types/uint64_t (c++ refs for each type)
https://en.cppreference.com/w/cpp/types/ptrdiff_t
https://en.cppreference.com/w/cpp/types/integer

https://doc.rust-lang.org/reference/types/numeric.html (rust refs for each type)
https://faultlore.com/blah/tower-of-weakenings/#strict-provenance-no-more-getting-lucky (provenance issues rust)


https://devblogs.microsoft.com/oldnewthing/20200728-00/?p=104012 (16 bit windows memory model history)
https://bumbershootsoft.wordpress.com/2022/08/05/memory-models-and-far-pointers/ (more 16-bit history)
https://www.youtube.com/watch?v=BBNdbDt3OmU (CHERI presentation)
https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-947.pdf (CHERI C/C++ programming guide)
https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-927.pdf (Super-detailed CHERI ISA)
https://www.thegoodpenguin.co.uk/blog/introducing-arm-morello-cheri-architecture/ (129-bit CHERI mem layout)
https://developer.apple.com/documentation/swift/special-use-numeric-types
* Swift number types