#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.hpp"
#include "num.hpp"

namespace mem {
    
    template <typename T>
    Error alloc(T **ptr, size_t count = 1) {
        *ptr = (T *)malloc(sizeof(T) * count);
        try_ptr(*ptr, Error::OutOfMemory);
        return Error::Okay;
    }

    template <typename T>
    Error zero_alloc(T **ptr, size_t count = 1) {
        *ptr = (T *)calloc(sizeof(T), count);
        try_ptr(ptr, Error::OutOfMemory);
        return Error::Okay;
    }

    template <typename T>
    Error resize_alloc(T **ptr, size_t count = 1) {
        T *tmp = (T *)realloc(*ptr, sizeof(T) * count);
        try_ptr(tmp, Error::OutOfMemory);
        *ptr = tmp;
        return Error::Okay;
    }

    template <typename T>
    void free(T **ptr) {
        ::free(*ptr); // to avoid calling the wrapper free function!
        *ptr = NULL;
    }

    template <typename T>
    void copy(T *dst, T *src, size_t count = 1) {
        memcpy(dst, src, sizeof(T) * count);
    }

    template <typename T>
    void move(T *dst, T *src, size_t count = 1) {
        memmove(dst, src, sizeof(T) * count);
    }

    template <typename T>
    void zero(T *ptr, size_t count = 1) {
        memset(ptr, 0, sizeof(T) * count);
    }

    template <typename T>
    void fill(T *ptr, T value, size_t count = 1) {
        for(size_t i = 0; i < count; ++i) {
            *(ptr + i) = value;
        }
    }

    template <char *>
    void fill(char *ptr, char value, size_t count) {
        memset(ptr, value, sizeof(char) * count);
    }

    template <uint8_t>
    void fill(uint8_t *ptr, uint8_t value, size_t count) {
        memset(ptr, value, sizeof(uint8_t) * count);
    }

    template <int8_t>
    void fill(int8_t *ptr, int8_t value, size_t count) {
        memset(ptr, value, sizeof(int8_t) * count);
    }
    
    template <typename T>
    void swap(T *a, T *b, size_t count = 1) {
        // TODO: use Duff's device
        // TODO: offer verson w/ tmp variable
        // TODO: consider intrinsics version
        uint64_t size = sizeof(T) * count;
        int i;
        int* ai = (int*)a;
        int* bi = (int*)b;
        while(size >= sizeof(i)) {
            i = *ai;
            *ai = *bi;
            *bi = i;
            ai++;
            bi++;
            size -= sizeof(i);
        }

        char* ac = (char*)ai;
        char* bc = (char*)bi;
        char c;
        while(size > 0) {
            c = *ac;
            *ac = *bc;
            *bc = c;
            ac++;
            bc++;
            size--;
        }
    }

    template <typename T>
    bool compare(T *a, T *b, size_t count = 1) {
        return !(bool)memcmp(a, b, sizeof(T) * count);
    }
}