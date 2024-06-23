#pragma once
#include "error.hpp"
#include "mem.hpp"
#include "num.hpp"
#include "fmt.hpp"

template<typename T>
class Vector {
    public:
        T *ptr;
        size_t len;
        size_t cap;

    private:
        Error resize(size_t new_cap) {
            /*
            What should the resize policy be?
            * When nothing is allocated, should be NULL ptr and empty!
            * Initial (alloc) size of 1
            * Growth rate of 2 (allows bitwise ops like << 1 instead of * 2)
            * Shrink rate of 2 (allow bitwise ops like >> 1 instead of / 2)
            * Shrink threshold of 1/2
            */
            Error error = Error::Okay;
            try_error(mem::resize_alloc(&this->ptr, new_cap));
            if(this->len > new_cap) {
                this->len = new_cap;
            }
            this->cap = new_cap;
            return error;
        }

        Error grow_if_needed() {
            Error error = Error::Okay;
            if(this->len == 0) {
                try_error(mem::alloc(&this->ptr));
                this->cap = 1;
                return error;
            }
            if(this->len >= this->cap) {
                size_t new_cap = this->cap << 1;
                return this->resize(new_cap);
            }
            return error;
        }

        Error shrink_if_needed() {
            Error error = Error::Okay;
            if(this->len == 0) {
                mem::free(&this->ptr);
                this->cap = 0;
                return error;
            }
            if(this->len <= this->cap >> 1) { // shrink threshold of 0.5
                size_t new_cap = this->cap >> 1;
                return this->resize(new_cap);
            }
            return error;
        }

    public:
        void init() {
            this->ptr = NULL;
            this->len = 0;
            this->cap = 0;
        }

        void fini() {
            mem::free(&this->ptr);
            this->len = 0;
            this->cap = 0;
        }

        T *at_unsafe(size_t index) {
            return this->ptr + index;
        }

        T *back_unsafe() {
            return this->at_unsafe(this->len - 1);
        }

        T *front_unsafe() {
            return this->at_unsafe(0);
        }

        Error check_index(size_t index) {
            bool valid = index < this->len;
            if(!valid) {
                return Error::InvalidIndex;
            }
            return Error::Okay;
        }

        Error at(size_t index, T **item_ptr) {
            Error error = Error::Okay;
            try_error(this->check_index(index));
            *item_ptr = this->at_unsafe(index);
            return error;
        }

        Error back(T **item_ptr) {
            Error error = Error::Okay;
            try_error(this->at(this->len - 1, item_ptr));
            return error;
        }

        Error front(T **item_ptr) {
            Error error = Error::Okay;
            try_error(this->at(0, item_ptr));
            return error;
        }

        Error get_at(size_t index, T *item) {
            Error error = Error::Okay;
            T *ptr = NULL;
            try_error(this->at(index, &ptr));
            mem::copy(item, ptr, 1);
            return error;
        }

        Error get_back(T *item) {
            Error error = Error::Okay;
            T *ptr = NULL;
            try_error(this->back(&ptr));
            mem::copy(item, ptr);
            return error;
        }

        Error get_front(T *item) {
            Error error = Error::Okay;
            T *ptr = NULL;
            try_error(this->front(&ptr));
            mem::copy(item, ptr, 1);
            return error;
        }

        Error set_at(size_t index, T *item) {
            Error error = Error::Okay;
            T *ptr = NULL;
            try_error(this->at(index, &ptr));
            mem::copy(ptr, item, 1);
            return error;
        }

        Error set_back(T *item) {
            Error error = Error::Okay;
            T *ptr = NULL;
            try_error(this->back(&ptr));
            mem::copy(ptr, item, 1);
            return error;
        }

        Error set_front(T *item) {
            Error error = Error::Okay;
            T *ptr = NULL;
            try_error(this->front(&ptr));
            mem::copy(ptr, item);
            return error;
        }

        Error push_at(size_t index, T *item) {
            Error error = Error::Okay;
            try_error(this->check_index(index));
            try_error(this->grow_if_needed());
            T *src = this->at_unsafe(index);
            T *dst = this->at_unsafe(index + 1);
            size_t count = this->len - index;
            mem::move(dst, src, count);
            mem::copy(src, item);
            this->len++;
            return error;
        }

        Error push_back(T *item) {
            Error error = Error::Okay;
            try_error(this->grow_if_needed());
            this->len++;
            T *back = NULL;
            try_error(this->back(&back));
            mem::copy(back, item);
            return error;
        }

        Error push_front(T *item) {
            return this->push_at(0, item);
        }

        Error pop_at(size_t index, T *item) {
            Error error = Error::Okay;
            try_error(this->get_at(index, item));
            try_error(this->delete_at(index));
            return error;
        }

        Error pop_back(T *item) {
            Error error = Error::Okay;
            try_error(this->get_back(item));
            try_error(this->delete_back());
            return error;
        }

        Error pop_front(T *item) {
            Error error = Error::Okay;
            try_error(this->get_front(item));
            try_error(this->delete_front());
            return error;
        }

        Error delete_at(size_t index) {
            Error error = Error::Okay;
            if(this->is_empty()) {
                return Error::PopEmpty;
            }
            try_error(this->check_index(index));
            size_t count = this->len - index - 1;
            T *src = this->at_unsafe(index + 1);
            T *dst = this->at_unsafe(index);
            mem::move(dst, src, count);
            this->len--;
            try_error(this->shrink_if_needed());
            return error;
        }

        Error delete_back() {
            Error error = Error::Okay;
            if(this->is_empty()) {
                return Error::PopEmpty;
            }
            this->len--;
            try_error(this->shrink_if_needed());
            return error;
        }

        Error delete_front() {
            return this->delete_at(0);
        }

        Error swap(size_t a, size_t b) {
            Error error = Error::Okay;
            T *a_ptr = NULL;
            T *b_ptr = NULL;
            try_error(this->at(a, &a_ptr));
            try_error(this->at(b, &a_ptr));
            mem::swap(a_ptr, b_ptr);
            return error;
        }

        bool is_empty() {
            return this->len == 0;
        }

        bool contains(T *item) {
            for(size_t i = 0; i < this->len; ++i) {
                T *test = this->at_unsafe(i);
                int check = mem::compare(test, item);
                if(check == 0) {
                    return true;
                }
            }
            return false;
        }

        Error find(T *item, size_t *index) {
            for(size_t i = 0; i < this->len; ++i) {
                T *test = this->at_unsafe(i);
                int check = mem::compare(test, item);
                if(check == 0) {
                    *index = i;
                    return Error::Okay;
                }
            }
            return Error::NotFound;
        }
};