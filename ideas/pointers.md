The use of symbols & to get ptr and * to deref sucks
It is confusing to learn but we put up with it because C.
Options for type decls:
* 
^ 
& 
ptr

let a = 5
let a_ptr = a.ptr() // get pointer
let a_val = a_ptr.val() // get value (dereference)

Pointer can get address (addr != ptr in CHERI)
ptr.get_iaddr()
ptr.get_uaddr()