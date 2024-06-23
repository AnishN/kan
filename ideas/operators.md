Basic math ops:
+ add()
- sub()
* mul()
/ div()
<< shl()
>> shr()
% rem()
%% mod()

Only plan to support two's complement atm.
* not sign-magnitude or one's complement
* In two's complement, addition and subtraction can be performed using the same circuitry as for unsigned numbers, which makes the hardware more efficient. 
* Additionally, two's complement has only one representation for zero, unlike sign-magnitude and ones' complement, which simplifies comparison operations. 
* This representation also allows for easier handling of overflow and underflow conditions. 
* Overall, two's complement is the most efficient and practical representation for performing arithmetic operations in computer systems.

* Remainder
    * not the same as modulo
    * means truncation towards 0
    * this is what % is in C
    * differs from mod only when dividend is -ve
    +7 rem +3 --> +1
    +7 rem -3 --> +1
    -7 rem +3 --> -1
    -7 rem -3 --> -1
    +7 mod +3 --> +1
    +7 mod -3 --> +1
    -7 mod +3 --> +2
    -7 mod -3 --> +2

default (no prefix):
    * can overflow
    * can cause div by 0 errors
check: 
    * return None instead of invalid result
    * div by zero
    * overflow/underflow
wrap: use two's complement wrapping behavior
sat: clamped to the numeric bounds instead of overflowing

a +% b  wrap_add
a -% b  wrap_sub
a *% b  wrap_mul
a -% b  wrap_neg

a +| b  sat_add
a -| b  sat_sub
a *| b  sat_mul
a <<| b sat_shl

Bitwise ops:
! bit_not()
& bit_and()
| bit_or()
^ bit_xor()

Logical ops:
not log_not()
and log_and()
or log_or()
xor log_xor()

Comparison ops:
==	eq()	Equality	
!=	ne()	Non-equality	
<	lt()	Less than	
<=	le()	Less than or equal	
>=	ge()	Greater than or equal	
>	gt()	Greater than	

References:
* https://tutorial.ponylang.io/expressions/ops.html
    * Just saying ops are unsafe is not enough
    * Love the idea of no canned operator precedence
    * Just unary binds tighter than infix/binary (aka modified Smalltalk!)
* https://stackoverflow.com/questions/65617367/what-are-partial-and-unsafe-operators
    * Normal versions are safe and return "sensible" values
    * Unsafe versions just are undefined (e.g. overflow, div by 0)
    * Partial version raise detectable errors at runtime
* https://ziglang.org/documentation/master/#Operators
    * Like having wrapping (wrap) vs saturating (sat) ops
    * Clamping (clamp) seems more intuitive for me
* https://stackoverflow.com/questions/13683563/whats-the-difference-between-mod-and-remainder
* https://bigmachine.io/theory/mod-and-remainder-are-not-the-same/
    * Think the article is confusd in the middle
* https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/divmodnote-letter.pdf
