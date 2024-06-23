Aliasing model rules (weak to strong):
* Callee-saved values pushed to the stack are anonymous (return pointer, frame pointer).
* “Scratch” values the compiler spills to the stack are anonymous.
* A newly declared variable is unaliased until a reference is taken to it.
* The memory returned by malloc is unaliased.
* Fields of a struct do not alias eachother (bitfields are made of sadness).
* Padding bytes are vaguely anonymous (messy because of memcpy/memset/unions/punning).
* Immutable variables are functionally unaliased in that they can never change values.
* In Rust, &mut is unaliased (Stacked Borrows).
* In C(++), T* and U* cannot alias if T!=U and neither is char (Strict Aliasing).

Need to define:
* Allocations (stack/heap alloc is unaliased, having 1 true name)
* Pointer Provenance (track the chain of custody to access the allocation sandbox)
    * should track back to the 1 true name
    * if you track this, you prove that 2 ptrs do not alias
        * good for performance if you actively can track this

References:
https://faultlore.com/blah/fix-rust-pointers/
https://rust-unofficial.github.io/too-many-lists/fifth-stacked-borrows.html