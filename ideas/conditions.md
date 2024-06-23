I guess my question is, should block exprs (e.g. if) be expressions?
* The thing that bugs me about that, is that it is not clear that they return values
* perhaps I could add a keyword, `yield`, that makes the block spit out a value
    * like `return` but at the block rather than func level
    * becomes very verbose
* If expressions allow you to preserve constancy though
    * Is that a real argument? Why not assign to uninitialized data?
    * Writing the same variable and assigning it in each branch w/ statements is a pain
    * Avoids long variable names
    * I hate that there are multiple ways to write the same thing w/ expr-based code though...

* The trouble with whitespace and `if` and `else` only is that ternaries would require re-arrangement!
    * Python suffers from this for their ternaries:
    a = 8 if result == "win" else -8

What about the following?

if cond then:
    expr
elif cond then:
    expr
else:
    expr

Why then? It works with ternary syntax!
let score_delta = if result == "win" then 8 elif result == "draw" then 0 else -8

Note the use of colons in the multiline split form (ugh)
let score_delta = 
    if result == "win" then: 8 
    elif result == "draw" then: 0 
    else: -8

What about nested forms? Will have to mull that over...


References:
https://grantwinney.com/if-else-vs-switch-case-pattern-matching/
https://stackoverflow.com/questions/15317895/correct-syntax-for-if-statements-in-haskell