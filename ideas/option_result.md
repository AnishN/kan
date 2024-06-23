Is it worth shortening Option -> Opt
Same w/ Result -> Res
+ The length may make these tedious to type and run into 80 char limit
- But looks dumb (6 chars is reasonable to type vs 3)
- Already could have shorter version w/ postfix operators:
    * Option<I32> -> Opt<I32> -> I32?
    * Result<I32, Index_Error> -> Res<I32, Index_Error> -> I32!<Index_Error>

Should Result be Result{Type, Error} or Result{Type} w/ error implied from type?
* Should an error be assoc w/ each type?
    + Simpler to type
    - Less flexible
    - Error types are not limited to possibilities that occur in the function

Should I let postfix be used?
func get_item(a: Arr{I32}, index: USize) -> Option{I32}
versus
func get_item(a: Arr{I32}, index: USize) -> I32?