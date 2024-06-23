#pragma once

template <typename F>
struct DeferCtx {
	F f;
	DeferCtx(F f) : f(f) {}
	~DeferCtx() { f(); }
};

template <typename F>
DeferCtx<F> defer_func(F f) {
	return DeferCtx<F>(f);
}

#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x) DEFER_2(x, __COUNTER__)
#define defer(code) auto DEFER_3(_defer_) = defer_func([&](){code;})