#include "source.hpp"
#include "file.hpp"
#include "defer.hpp"

Error Source::init_from_path(String *path) {
    Error error = Error::Okay;
    File file;
    try_error(file.init(path, FileMode::RO));
    defer(file.fini());

    this->str.init_empty();
    try_error(file.read_string(&this->str));
    return error;
}

void Source::fini() {
    this->str.fini();
}

void Source::print_token_text(Token *token) {
    for(size_t i = 0; i < token->len; ++i) {
        char c = this->str.at_unsafe(token->start + i);
        switch(c) {
            case '\n': 
                fmt::print("\\n");
                break;
            case '\t': 
                fmt::print("\\t");
                break;
            case '\0':
                fmt::print("\\0");
                break;
            default:
                fmt::print(c);
                break;
        }
    }
}

void Source::print_token_full(Token *token) {
    fmt::printf("{} ({}-{}) '", 
        token->kind.get_ptr(), token->start, token->start + token->len);
    this->print_token_text(token);
    fmt::println("'");
}