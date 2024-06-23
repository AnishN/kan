type_limit Float:
    F16
    F32
    F64

@where.type_default(T, F32)
@where.type_limit(T, Float)
// @where.type_limit(T, F16 | F32 | F64)
struct Point{T}:
    x: T
    y: T
    z: T