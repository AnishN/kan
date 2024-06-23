#pragma once
#include "error.hpp"
#include "hash.hpp"
#include "num.hpp"
#include "vector.hpp"

template<typename K, typename V>
class HashMap {
    private:
        class Pair {
            K key;
            V value;
        };
        Vector<Pair> pairs;

    private:
        uint64_t hash(K *key) {
            return hash::crc64(key); 
        }

    public:
        void init() {
            this->pairs.init();
        }

        void fini() {
            this->pairs.fini();
        }

        Error at(K *key, V **value_ptr) {
            Error error = Error::Okay;
            
            return error;
        }

        Error get(K *key, V *value) {
            Error error = Error::Okay;
            return error;
        }

        Error set(K *key, V *value) {
            Error error = Error::Okay;
            return error;
        }

        Error insert(K *key, V *value) {
            Error error = Error::Okay;
            return error;
        }

        Error remove(K *key, V *value) {
            Error error = Error::Okay;
            return error;
        }

        Error swap(V *a, V *b) {
            Error error = Error::Okay;
            return error;
        }

        bool is_empty() {
            return false;
        }

        bool contains(K *key) {
            return false;
        }
};
