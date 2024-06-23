#include "str.hpp"
#include "bit.hpp"
#include "fmt.hpp"
#include "mem.hpp"

bool String::is_char_alpha(char c) {
    if(
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z')
    ) {
        return true;
    }
    return false;
}

bool String::is_char_digit(char c) {
    if(c >= '0' && c <= '9') {
        return true;
    }
    return false;
}

void String::init_empty() {
    this->data.ptr = NULL;
    this->data.len = 0;
    this->data.cap = 0;
}

Error String::init_from_ptr(const char *ptr) {
    size_t len = strlen(ptr);
    return this->init_from_ptr_len(ptr, len);
}

Error String::init_from_len(size_t len) {
    Error error = Error::Okay;
    try_error(mem::zero_alloc(&this->data.ptr, len + 1));
    this->data.len = len;
    this->data.cap = bit_next_pow2_uint64(this->data.len + 1);
    return error;
}

Error String::init_from_ptr_len(const char *ptr, size_t len) {
    Error error = Error::Okay;
    try_error(mem::alloc(&this->data.ptr, len + 1));
    mem::copy(this->data.ptr, (char *)ptr, len + 1);
    this->data.ptr[len] = '\0';
    this->data.len = len;
    this->data.cap = bit_next_pow2_uint64(this->data.len + 1);
    return error;
}

Error String::init_from_string(String *src) {
    Error error = Error::Okay;
    try_error(this->init_from_ptr_len(
        (const char *)src->data.ptr, 
        src->data.len)
    );
    return error;
}

void String::fini() {
    mem::free(&this->data.ptr);
    this->data.len = 0;
    this->data.cap = 0;
}

const char *String::get_ptr() {
    return (const char *)this->data.ptr;
}

size_t String::get_len() {
    return this->data.len;
}

char String::at_unsafe(size_t index) {
    return *this->data.at_unsafe(index);
}

Error String::check_index(size_t index) {
    return this->data.check_index(index);
}

Error String::at(size_t index, char *c) {
    Error error = Error::Okay;
    try_error(this->data.check_index(index));
    *c = this->at_unsafe(index);
    return error;
}

void String::print() {
    fmt::print(this->get_ptr());
}

void String::eprint() {
    fmt::eprint(this->get_ptr());
}