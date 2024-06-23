I think the only 2 uses of goto that are interesting are:
1. FSMs (computed goto)
2. Error handling (so forward jumps only)
    * At end of func vs scope
    * On error or under certain conditions
    * the jumping spots always happen at the end
    * `defer` handles at end of scope
    * `final` could handle at end of func
    * `clean` as another alt keyword

Why could computed goto help the branch predictor?
https://forum.nim-lang.org/t/4347
The point of computed goto it not that an array lookup can be used, compilers do that for switch too and Nim helps the C compilers by injecting something like default: unreachable() for exhaustive case statements, so that the index check before the array is accessed can be eliminated. (Something which the oh-so-efficient-C cannot do with its weakly typed enums.)

The point of computed goto is to duplicate the dispatching logic so that the branch predictors in the CPU have multiple different conditions to predict. This may sound weird, but consider that in an interpreter you have correlations between your opcodes, for example, "after a push instruction a call instruction is more likely to follow" and duplicating of the dispatching logic takes advantage of these correlations.

This only makes sense when your case is within a loop, it's not generally applicable for case statements.

@Araq is right, the main advantage of computed gotos is to better use the hardware indirect branch predictor if your case statement is done in a loop. Using a table instead would be a guaranteed cache miss.

The rest have been covered by our standard structured programmng constructs:
* e.g.: while and for loops

Crazy alt to non-standard C computed goto: "switched goto":
https://bullno1.com/blog/switched-goto

https://rubber-duck-typing.com/posts/2017-04-26-goto-the-marvelous.html
https://embeddedgurus.com/stack-overflow/2010/04/efficient-c-tip-12-be-wary-of-switch-statements/
https://www.cipht.net/2017/10/03/are-jump-tables-always-fastest.html

Other alternatives:
* Indirect threaded code: https://wiki.c2.com/?IndirectThreadedCode
* Direct threaded code: https://wiki.c2.com/?DirectThreadedCode
* Threaded code overview: 
    * https://www.complang.tuwien.ac.at/forth/threaded-code.html
    * https://jilp.org/vol5/v5paper12.pdf

The `goto` will not be unrestricted of course.
* You cannot jump out of the parent scope (block/func) the goto is invoked in.
* You cannot jump into a "nested" func/lambda.
* Jumping into a nested block should be fine.
    * What if the block is capturing?
I don't think renaming `goto` to somthing else (e.g. `jump`) makes sense.
* "Reducing the stigma" is a silly argument.
* Jump is a more useful word -> less ideal to reserve

int interp_cgoto(unsigned char* code, int initval) {
    static void* dispatch_table[] = {
        &&do_halt, &&do_inc, &&do_dec, &&do_mul2,
        &&do_div2, &&do_add7, &&do_neg};
    #define DISPATCH() goto *dispatch_table[code[pc++]]
    int pc = 0;
    int val = initval;

    DISPATCH();
    while (1) {
        do_halt:
            return val;
        do_inc:
            val++;
            DISPATCH();
        do_dec:
            val--;
            DISPATCH();
        do_mul2:
            val *= 2;
            DISPATCH();
        do_div2:
            val /= 2;
            DISPATCH();
        do_add7:
            val += 7;
            DISPATCH();
        do_neg:
            val = -val;
            DISPATCH();
    }
}

Other limited versions of `goto` are helpful:

* `defer` for executing expr/stmt later
    * unlike golang, this is deferred to the end of scope
    * simplest is to make it a block that anything within it is deferred
        * order of exec in the block is still forwards though lol

Zig does some odd stuff w/ defer:
* defer expr, evaluates expr when control flow leaves scope
* %defer expr, evaluates expr when control flow leaves scope by returning an error from the function
* ?defer expr, evaluates expr when control flow leaves scope by returning null from the function

Dlang does this:
scope (exit), scope (success), and scope (failure)


Switch (has order preserved - really is a match)
switch expr:
    case expr:
    case expr:
    case expr:
    default:

Branch (no order is preserved - really is a computed goto replacement)
branch expr:
    case expr:
    case expr:
    case expr:
    default:

func interp(codes: u8[], init_val: int32_t) -> int32_t:
    let val: int32_t = init_val
    branch code in codes.iter():
        case halt: return val
        case inc: val += 1
        case dec: val -= 1
        case mul_2: val *= 2
        case div_2: val /= 2
        case add_7: val += 7
        case neg: val = -val
 
https://itnext.io/goto-hell-1e7e32989092