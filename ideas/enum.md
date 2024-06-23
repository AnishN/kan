Multiple types:

enum Type_Name(int backing)
enum Piece(uint32_t):
    Pawn
    Knight
    Bishop
    Rook
    Queen
    King

union Type_Name
union Num:
    float32_t
    float64_t
    Uint32_t
    Uint64_t

variant Type_Name(int backing)
variant Event:
    Mouse_Event
    Key_Event{I32}

flag Type_Name(int backing)
flag (allows bitwise ops)

error Type_Name(int backing)
error (reserve zero as okay, cannot set values?)