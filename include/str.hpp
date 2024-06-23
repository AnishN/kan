#pragma once
#include "error.hpp"
#include "num.hpp"
#include "vector.hpp"

class String {
    public:
        // unfortunately enforces null-termination
        // since C APIs assume this
        // assumes ownership of its data
        // len does NOT include the null terminator!
        Vector<char> data;
    
    public:
        static bool is_char_alpha(char c);
        static bool is_char_digit(char c);
        void init_empty();
        Error init_from_ptr(const char *ptr);
        Error init_from_len(size_t len);
        Error init_from_ptr_len(const char *ptr, size_t len);
        Error init_from_string(String *src);
        void fini();
        const char *get_ptr();
        size_t get_len();
        char at_unsafe(size_t index);
        Error check_index(size_t index);
        Error at(size_t index, char *c);
        void print();
        void eprint();
};