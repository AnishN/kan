#include "lexer.hpp"

void Lexer::init(Source *source, Vector<Token> *tokens) {
    this->source = source;
    this->tokens = tokens;
    this->index = 0;
    this->len = 0;
    this->line = 0;
    this->col = 0;
}

void Lexer::fini() {
    this->source = NULL;
    this->tokens = NULL;
    this->index = 0;
    this->len = 0;
    this->line = 0;
    this->col = 0;
}

Error Lexer::run() {
    Error error = Error::Okay;
    this->tokens->init();
    this->index = 0;
    this->len = 0;
    this->line = 0;
    this->col = 0;

    while(true) {
        Token t = this->next_token();
        if(t.kind != TokenKind::Space) { // filter out useless space tokens
            try_error(this->tokens->push_back(&t));
        }
        switch(t.kind) {
            case TokenKind::EndOfFile:
                return error;
            case TokenKind::NewLine:
                this->line++;
                this->col = 0;
                break;
            case TokenKind::Space:
                this->col += t.len;
                break;
            default:
                this->col += t.len;
        }
    }
    return error;
}

Token Lexer::make_token(TokenKind kind) {
    Token token;
    token.init(kind, this->index - this->len, this->len, this->line, this->col);
    return token;
}

char Lexer::peek_char(size_t offset) {
    Error error = Error::Okay;
    char c;
    error = this->source->str.at(this->index + offset, &c);
    if(this->index + offset >= this->source->str.get_len()) {
        return '\0';
    }
    return this->source->str.at_unsafe(this->index);
}

char Lexer::next_char() {
    char next = this->peek_char();
    this->index++;
    this->len++;
    return next;
}

bool Lexer::match_char(char expect) {
    char peek = this->peek_char();
    if(peek == expect) {
        this->index++;
        this->len++;
        return true;
    }
    return false;
}

bool Lexer::match_string(const char *expect) {
    bool match = true;
    size_t count = 0;
    while(true) {
        char e = expect[count];
        if(e == '\0') {
            break;
        }
        char s = this->source->str.at_unsafe(this->index - this->len + count);
        if(s == '\0') {
            break;
        }
        if(s != e) {
            match = false;
            break;
        }
        count++;
    }
    return match;
}

bool Lexer::is_line_start() {
    return this->col == 0;
}

#define token_case(c, k) case c: kind = k; break;
#define token_case2(c, n, k_yes, k_no) \
    case c: \
        if(this->match_char(n)) { \
            kind = k_yes; \
        }\
        else { \
            kind = k_no; \
        }\
        break;
#define token_case_equal(c, k) token_case2(c, '=', k##Equal, k)
#define token_string_match(s, k) \
    if(this->match_string(s)) { \
        kind = k; \
    }

Token Lexer::next_token() {
    this->len = 0;
    char c = this->next_char();
    TokenKind kind = TokenKind::Invalid;
    switch(c) {
        token_case('\0', TokenKind::EndOfFile);
        token_case('(', TokenKind::LeftParen);
        token_case(')', TokenKind::RightParen);
        token_case('{', TokenKind::LeftBrace);
        token_case('}', TokenKind::RightBrace);
        token_case(',', TokenKind::Comma);
        token_case('.', TokenKind::Dot);
        token_case(';', TokenKind::Semicolon);
        token_case('+', TokenKind::Plus);
        token_case('-', TokenKind::Minus);
        token_case('*', TokenKind::Star);
        token_case('/', TokenKind::Slash);
        
        token_case_equal('!', TokenKind::Not);
        token_case_equal('=', TokenKind::Equal);
        token_case_equal('>', TokenKind::Greater);
        token_case_equal('<', TokenKind::Less);

        token_case('\t', TokenKind::Tab);
        token_case('\n', TokenKind::NewLine);
        case ' ': 
            kind = TokenKind::Space;
            if(this->match_string("    ")) {
                kind = TokenKind::Indent;
                this->index += 3;
            }

        default:
            if(String::is_char_alpha(c) || c == '_') {
                while(true) {
                    char p = this->peek_char();
                    if(p == '\0') {
                        break;
                    }
                    if(
                        String::is_char_alpha(p) ||
                        String::is_char_digit(p) ||
                        p == '_'
                    ) {}
                    else {
                        break;
                    }
                    this->next_char();
                }

                /*
                TODO: Use switch statement to match keywords instead
                This would avoid repeat work by matching with prefixes.
                Also should assume it is an identifier if it has num or _.
                */
                token_string_match("let", TokenKind::Let)
                else token_string_match("import", TokenKind::Import)
                else token_string_match("export", TokenKind::Export)
                else token_string_match("func", TokenKind::Func)
                else {
                    kind = TokenKind::Identifier;
                }
            }
            else if(String::is_char_digit(c)) {
                kind = TokenKind::Number;
            }
    }
    Token token = this->make_token(kind);
    return token;
}

void Lexer::print_tokens() {
    fmt::println("Lexer:");
    for(size_t i = 0; i < this->tokens->len; ++i) {
        Token *token = this->tokens->at_unsafe(i);
        fmt::printf("{}: ", i);
        this->source->print_token_full(token);
    }
    fmt::println("");
}