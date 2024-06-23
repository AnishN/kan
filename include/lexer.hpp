#pragma once
#include "error.hpp"
#include "source.hpp"
#include "token.hpp"
#include "vector.hpp"

class Lexer {
    private:
        Source *source;
        Vector<Token> *tokens;
        size_t index;
        size_t len;
        size_t line;
        size_t col;

        Token make_token(TokenKind kind);
        Token next_token();
        char peek_char(size_t offset = 0);
        char next_char();
        bool match_char(char expect);
        bool match_string(const char *expect);
        bool is_line_start();

    public:
        void init(Source *source, Vector<Token> *tokens);
        void fini();
        Error run();
        void print_tokens();
};