Here is how Jai does its closure/function/lambda syntax:

                                    { ... } // Anonymous code block
                        [capture] { ... } // Captured code block
        (i: int) -> float [capture] { ... } // Anonymous function
    f :: (i: int) -> float [capture] { ... } // Named local function
    f :: (i: int) -> float [capture] { ... } // Named global function

Note how the changes are incremental.
This is my parallel in a whitespace significant language:
    
    // Anonymous code block
    block: 
        pass

    // Named code block
    block b:
        pass

    // Anonymous captured code block
    block {}:
        pass

    // Named captured code block
    block b {}:
        pass

    // Anonymous function
    func (i: int) -> float:
        pass

    // Anonymous captured function
    func {} (i: int) -> float:
        pass

    // Named function
    func f (i: int) -> float:
        pass

    // Named captured function
    func f {} (i: int) -> float:
        pass

What about def instead of func?
* Aligns nicely with let :)
* Not as obvious/memorable though...

An alternative for `block` could be `scope`:
    * Feel like that word is less common to be a collision in code.
    * I still think of the construct as a block mentally though, so maybe no.

I don't think :: is good (since that may be used for module member access.
* Plus it just does not stand out well when scanning code.
* Looks like sigil soup :)

I could just make `block` and `func` into a single keyword (? `proc` or `def`).
Alternatively, I could reuse `let` since all these are declarative constructs.
* (+) That would reduce diffs/typing.
* (-) This would make it harder to grep for the block/func.
* (-) Also, block and func have diff semantics (if I include goto).