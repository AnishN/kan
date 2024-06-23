#pragma once
#include "fmt.hpp"
#include "num.hpp"
#include <stdlib.h>

class Error
{
    public:
        enum Value {
            Okay,
            OutOfMemory,
            InvalidIndex,
            PopEmpty,
            NotFound,
            InvalidPath,
        };

        Error() = default;
        constexpr Error(Value v) : value(v) { }
        constexpr operator Value() const { return value; }
        explicit operator bool() const = delete;

        void print() {
            fmt::printf("{} ({})", (uint64_t)this->value, this->get_ptr());
        }

        void eprint() {
            fmt::eprintf("{} ({})", (uint64_t)this->value, this->get_ptr());
        }

        constexpr const char *get_ptr() const { 
            switch(this->value) {
                case Error::Okay: return "okay";
                case Error::OutOfMemory: return "out of memory";
                case Error::InvalidIndex: return "invalid index";
                case Error::PopEmpty: return "pop empty";
                case Error::NotFound: return "not found";
                case Error::InvalidPath: return "invalid path";
            };
            return "";
        }

    private:
        Value value;
};

#define try_error(expr) \
    do { \
        Error error = expr; \
        if(error != Error::Okay) { \
            return error; \
        } \
    } while(0); 

#define try_ptr(ptr, error) \
    do { \
        if(ptr == NULL) { \
            return error; \
        } \
    } while(0);
