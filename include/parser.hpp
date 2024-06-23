#pragma once
#include "source.hpp"
#include "token.hpp"
#include "vector.hpp"

enum class Precedence {
    None,
    Assignment, // =
    Or, // or
    And, // and
    Equality, // == !=
    Comparison, // < > <= >=
    Term, // + -
    Factor, // * /
    Unary, // ! -
    Call, // . ()
    Primary,
};

class Parser;
typedef void (Parser::*ParseFunc)();

class ParserRule {
    public:
        ParseFunc prefix;
        ParseFunc infix;
        Precedence precedence;
};

class Parser {
    private:
        Source *source;
        Vector<Token> *tokens;
        size_t prev_index;
        size_t curr_index;

    public:
        void init(Source *source, Vector<Token> *tokens);
        void fini();
        void run();

        Token *get_prev_token();
        Token *get_curr_token();
        void advance();
        void consume(TokenKind kind, const char *msg);
        bool is_at_end();

        void expr();
        void number_expr();
        void group_expr();
        void unary_expr();
        void binary_expr();
        ParserRule *get_rule(TokenKind kind);
        void parse_precedence(Precedence precedence);
        // void emit_constant(float64_t val);
};