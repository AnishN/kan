constructs I am debating:
* `loop`
    * I love the simplicity of it over `while true`
    * Completely subjective -  I abuse this in rust
    * what about `while ... else` construct (python)?
        * It has a use: only executed when while cond is false
            * aka when you exit the loop block normally
            * **not** if you `break` or `return` in the loop
            * saves you from checking a random bool
* `while cond`
* `for x in iter`
    * what about `for ... else` construct (python)? same value as `while else`
* `break` and `continue` in any of the loop constructs
    * should it be loop based or block based?

* `do_while`
    * alt names? `while_pre` and `while_post`
        * nah: the former is very common, and two_word keywords suck
    * doing it C-style w/ cond @ end look gross
    do:
        blah()
        foo()
        bar()
    while(***)

    * can emulate in several ways, including:
        loop:
            do_stuff()
            if cond == false:
                break
        
        loop:
            defer:
                if cond == false:
                    break
            do_stuff()