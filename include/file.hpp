#pragma once
#include <cstdio>
#include "str.hpp"

enum class FileMode {
    RO,
    WO,
    RW,
};

class File {
    private:
        String path;
        FILE *handle;
        FileMode mode;

    public:
        Error init(String *path, FileMode mode);
        void fini();
        uint64_t get_size();
        bool is_at_end();
        Error read_string(String *string);
};