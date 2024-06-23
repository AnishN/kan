#pragma once
#include "fmt.hpp"

class TokenKind {
    public:
        enum Value {
            Invalid,
            EndOfFile,
            Indent,
            Space,
            NewLine,
            Tab,
            LeftParen,
            RightParen,
            LeftBrace,
            RightBrace,
            Comma,
            Dot,
            Colon,
            Semicolon,
            Plus,
            Minus,
            Star,
            Slash,
            Equal,
            EqualEqual,
            Not,
            NotEqual,
            Greater,
            GreaterEqual,
            Less,
            LessEqual,
            Identifier,
            Number, 
            String,
            Let,
            Import,
            Export,
            Func,
        };

        TokenKind() = default;
        constexpr TokenKind(Value v) : value(v) { }
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
                case Invalid: return "invalid";
                case EndOfFile: return "end of file";
                case Indent: return "indent";
                case Space: return "space";
                case NewLine: return "new line";
                case Tab: return "tab";
                case LeftParen: return "left paren";
                case RightParen: return "right paren";
                case LeftBrace: return "left brace";
                case RightBrace: return "right brace";
                case Comma: return "comma";
                case Dot: return "dot";
                case Colon: return "colon";
                case Semicolon: return "semicolon";
                case Plus: return "plus";
                case Minus: return "minus";
                case Star: return "star";
                case Slash: return "slash";
                case Equal: return "equal";
                case EqualEqual: return "equal equal";
                case Not: return "not";
                case NotEqual: return "not equal";
                case Greater: return "greater";
                case GreaterEqual: return "greater equal";
                case Less: return "less";
                case LessEqual: return "less equal";
                case Identifier: return "identifier";
                case Number: return "number";
                case String: return "string";
                case Let: return "let";
                case Import: return "import";
                case Export: return "export";
                case Func: return "func";
            };
            return "";
        }

    private:
        Value value;
};

class Token {
    public:
        TokenKind kind;
        size_t start;
        size_t len;
        size_t line;
        size_t col;

    public:
        void init(
            TokenKind kind, size_t start, size_t len, size_t line, size_t col) {
            this->kind = kind;
            this->start = start;
            this->len = len;
            this->line = line;
            this->col = col;
        }
};