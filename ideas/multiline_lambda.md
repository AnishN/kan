func rand_byte() -> uint8_t:
    pass

struct Point:
    x: float32_t
    y: float32_t

enum Ordering:
    Less = -1
    Equal = 0
    Greater = 1

func main() -> Void:
    let a = rand_byte()
    println(a)
    let b = Vector::init([1, 7, 6])
    b.sort_by(true, 
        func _ (a: Point, b: Point) -> Ordering:
            println(a.x)
            println(b.x) 
            return (a.x).cmp(b.x)
        , 11,
    )

    func sort_x (a: Point, b: Point) -> Ordering: 
        return (a.x).cmp(b.x)
    b.sort_by(true, sort_x, 11)
    print