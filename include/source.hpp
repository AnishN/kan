#pragma once
#include "error.hpp"
#include "str.hpp"
#include "token.hpp"

class Source {
    public:
        String str;

        Error init_from_path(String *path);
        void fini();
        void print_token_text(Token *token);
        void print_token_full(Token *token);
};