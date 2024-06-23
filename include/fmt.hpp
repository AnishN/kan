#pragma once
#include <concepts>
#include <type_traits>
#include <vector>
#include <cstdio>
#include "num.hpp"

namespace fmt {

    template <typename T>
    concept has_print_func = requires(T arg) {
        { arg.print() } -> std::same_as<void>;
    };

    template <typename T>
    void _print(T arg) {
        if constexpr(std::is_same_v<T, char>) {
            ::printf("%c", arg);
        }
        else if constexpr(std::is_same_v<T, bool>) {
            if(arg) {
                ::printf("true");
            }
            else {
                ::printf("false");
            }
        }
        else if constexpr(std::is_integral_v<T>) {
            if constexpr(std::is_signed_v<T>) {
                ::printf("%jd", (intmax_t)arg);
            }
            else {
                ::printf("%ju", (uintmax_t)arg);
            }
        }
        else if constexpr(std::is_floating_point_v<T>) {
            ::printf("%f", arg);
        }
        else if constexpr(std::is_same_v<T, const char *>) {
            ::printf("%s", arg);
        }
        else if constexpr(has_print_func<T>) {
            arg.print();
        }
    }

    template <typename T>
    void _printf(const char **fmt, T arg) {
        uint64_t i = 0;
        while(true) {
            char curr = (*fmt)[i];
            char next = (*fmt)[i + 1];
            if(curr == '\0') {
                break;
            }
            else if(next == '\0') { 
                fputc(curr, stdout);
                break;
            }
            else if(curr == '{' && next == '}') {
                _print(arg);
                *fmt = &((*fmt)[i + 2]);
                break;
            }
            else {
                fputc(curr, stdout);
                ++i;
            }
        }
    }

    template <typename T, typename ...Args>
    void _printf(const char **fmt, T first, Args... rest) {
        _printf(fmt, first);
        _printf(fmt, rest...);
    }

    template <typename ...Args>
    void printf(const char *fmt, Args... args) {
        _printf(&fmt, args...);
        ::printf("%s", fmt);
    }

    template <typename ...Args>
    void printlnf(const char *fmt, Args... args) {
        printf(fmt, args...);
        fputc('\n', stdout);
    }

    template <typename T, typename ...Args>
    void _print(T first, Args... rest) {
        _print(first);
        fputc(' ', stdout);
        _print(rest...);
    }

    template <typename ...Args>
    void print(Args... args) {
        _print(args...);
    }

    template <typename ...Args>
    void println(Args... args) {
        print(args...);
        fputc('\n', stdout);
    }


    template <typename T>
    concept has_eprint_func = requires(T arg) {
        { arg.eprint() } -> std::same_as<void>;
    };

    template <typename T>
    void _eprint(T arg) {
        if constexpr(std::is_same_v<T, char>) {
            ::fprintf(stderr, "%c", arg);
        }
        else if constexpr(std::is_same_v<T, bool>) {
            if(arg) {
                ::printf("true");
            }
            else {
                ::printf("false");
            }
        }
        else if constexpr(std::is_integral_v<T>) {
            if constexpr(std::is_signed_v<T>) {
                ::fprintf(stderr, "%jd", (intmax_t)arg);
            }
            else {
                ::fprintf(stderr, "%ju", (uintmax_t)arg);
            }
        }
        else if constexpr(std::is_floating_point_v<T>) {
            ::fprintf(stderr, "%f", arg);
        }
        else if constexpr(std::is_same_v<T, const char *>) {
            ::fprintf(stderr, "%s", arg);
        }
        else if constexpr(has_eprint_func<T>) {
            arg.eprint();
        }
    }

    template <typename T>
    void _eprintf(const char **fmt, T arg) {
        uint64_t i = 0;
        while(true) {
            char curr = (*fmt)[i];
            char next = (*fmt)[i + 1];
            if(curr == '\0') {
                break;
            }
            else if(next == '\0') { 
                fputc(curr, stderr);
                break;
            }
            else if(curr == '{' && next == '}') {
                _eprint(arg);
                *fmt = &((*fmt)[i + 2]);
                break;
            }
            else {
                fputc(curr, stderr);
                ++i;
            }
        }
    }

    template <typename T, typename ...Args>
    void _eprintf(const char **fmt, T first, Args... rest) {
        _eprintf(fmt, first);
        _eprintf(fmt, rest...);
    }

    template <typename ...Args>
    void eprintf(const char *fmt, Args... args) {
        _eprintf(&fmt, args...);
        ::fprintf(stderr, "%s", fmt);
    }

    template <typename ...Args>
    void eprintlnf(const char *fmt, Args... args) {
        eprintf(fmt, args...);
        fputc('\n', stderr);
    }

    template <typename T, typename ...Args>
    void _eprint(T first, Args... rest) {
        _eprint(first);
        fputc(' ', stderr);
        _eprint(rest...);
    }

    template <typename ...Args>
    void eprint(Args... args) {
        _eprint(args...);
    }

    template <typename ...Args>
    void eprintln(Args... args) {
        eprint(args...);
        fputc('\n', stderr);
    }
}