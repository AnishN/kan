#include "parser.hpp"
#include "fmt.hpp"

namespace Rules {
    typedef TokenKind Kind;
    typedef Precedence Prec;
    typedef Parser P;

    ParserRule rules[] = {
        [Kind::Invalid] = {NULL, NULL, Prec::None},
        [Kind::EndOfFile] = {NULL, NULL, Prec::None},
        [Kind::Indent] = {NULL, NULL, Prec::None},
        [Kind::Space] = {NULL, NULL, Prec::None},
        [Kind::NewLine] = {NULL, NULL, Prec::None},
        [Kind::Tab] = {NULL, NULL, Prec::None},
        [Kind::LeftParen] = {&P::group_expr, NULL, Prec::None},
        [Kind::RightParen] = {NULL, NULL, Prec::None},
        [Kind::LeftBrace] = {NULL, NULL, Prec::None},
        [Kind::RightBrace] = {NULL, NULL, Prec::None},
        [Kind::Comma] = {NULL, NULL, Prec::None},
        [Kind::Dot] = {NULL, NULL, Prec::None},
        [Kind::Colon] = {NULL, NULL, Prec::None},
        [Kind::Semicolon] = {NULL, NULL, Prec::None},
        [Kind::Plus] = {NULL, &P::binary_expr, Prec::Term},
        [Kind::Minus] = {&P::unary_expr, &P::binary_expr, Prec::Term},
        [Kind::Star] = {NULL, &P::binary_expr, Prec::Factor},
        [Kind::Slash] = {NULL, &P::binary_expr, Prec::Factor},
        [Kind::Equal] = {NULL, NULL, Prec::None},
        [Kind::EqualEqual] = {NULL, NULL, Prec::None},
        [Kind::Not] = {NULL, NULL, Prec::None},
        [Kind::NotEqual] = {NULL, NULL, Prec::None},
        [Kind::Greater] = {NULL, NULL, Prec::None},
        [Kind::GreaterEqual] = {NULL, NULL, Prec::None},
        [Kind::Less] = {NULL, NULL, Prec::None},
        [Kind::LessEqual] = {NULL, NULL, Prec::None},
        [Kind::Identifier] = {NULL, NULL, Prec::None},
        [Kind::Number] = {&P::number_expr, NULL, Prec::None},
        [Kind::String] = {NULL, NULL, Prec::None},
        [Kind::Let] = {NULL, NULL, Prec::None},
        [Kind::Import] = {NULL, NULL, Prec::None},
        [Kind::Export] = {NULL, NULL, Prec::None},
        [Kind::Func] = {NULL, NULL, Prec::None},
    };
};

void Parser::init(Source *source, Vector<Token> *tokens) {
    this->source = source;
    this->tokens = tokens;
    this->prev_index = 0;
    this->curr_index = 0;
}

void Parser::fini() {
    this->source = NULL;
    this->tokens = NULL;
    this->prev_index = 0;
    this->curr_index = 0;
}

void Parser::run() {
    fmt::println("Parser:");
    this->prev_index = 0;
    this->curr_index = 0;
    while(!this->is_at_end()) {
        this->expr();
    }
    fmt::println("Success!");
    fmt::println("");
}

Token *Parser::get_prev_token() {
    return this->tokens->at_unsafe(prev_index);
}

Token *Parser::get_curr_token() {
    return this->tokens->at_unsafe(curr_index);
}

void Parser::advance() {
    while(true) {
        Token *curr_token = this->get_curr_token();
        this->prev_index = this->curr_index;
        this->curr_index++;
        if(curr_token->kind == TokenKind::EndOfFile) {
            break;
        }
        else if(curr_token->kind != TokenKind::Invalid) {
            break;
        }
        fmt::eprintln("error");
    }
}

void Parser::consume(TokenKind kind, const char *msg) {
    Token *curr_token = this->get_curr_token();
    if(curr_token->kind != kind) {
        fmt::eprintln(msg);
        fmt::eprintlnf("expected {}, found {}", kind, curr_token->kind);
        return;
    }
    this->advance();
}

bool Parser::is_at_end() {
    return this->curr_index >= this->tokens->len;
}

void Parser::expr() {
    this->parse_precedence(Precedence::Assignment);
}

void Parser::number_expr() {
    Token *prev_token = this->get_prev_token();
    char *text_ptr = (char *)this->source->str.get_ptr() + prev_token->start;
    float64_t val = strtod(text_ptr, NULL);
    fmt::printlnf("number_expr: {}", val);
}

void Parser::group_expr() {
    this->expr();
    this->consume(TokenKind::RightParen, "Expect ')' after expression.");
}

void Parser::unary_expr() {
    TokenKind op_kind = this->get_prev_token()->kind;
    this->parse_precedence(Precedence::Unary);
    switch(op_kind) {
        case TokenKind::Minus: {
            fmt::println("unary_op: negate");
            break;
        }
        default: return;
    }
}

void Parser::binary_expr() {
    TokenKind op_kind = this->get_prev_token()->kind;
    ParserRule *rule = this->get_rule(op_kind);
    Precedence one_up = (Precedence)((size_t)rule->precedence + 1);
    this->parse_precedence(one_up);

    switch(op_kind) {
        case TokenKind::Plus: {
            fmt::println("binary_op: add"); 
            break;
        }
        case TokenKind::Minus: {
            fmt::println("binary_op: sub"); 
            break;
        }
        case TokenKind::Star: {
            fmt::println("binary_op: mul"); 
            break;
        }
        case TokenKind::Slash: {
            fmt::println("binary_op: div"); 
            break;
        }
        default: return; // Unreachable.
    }
}

ParserRule *Parser::get_rule(TokenKind kind) {
    // fmt::printlnf("get_rule {}", kind);
    return &Rules::rules[kind];
}

void Parser::parse_precedence(Precedence precedence) {
    this->advance();
    Token *prev_token = this->get_prev_token();
    // fmt::printlnf("parse_precedence: rule {}", prev_token->kind);
    ParseFunc prefix_rule = this->get_rule(prev_token->kind)->prefix;
    if(prefix_rule == NULL) {
        // fmt::eprintln("Expected expression.");
        return;
    }
    (this->*prefix_rule)();
    
    while(true) {
        Token *curr_token = this->get_curr_token();
        if(precedence > this->get_rule(curr_token->kind)->precedence) {
            break;
        }
        this->advance();
        Token *prev_token = this->get_prev_token();
        ParseFunc infix_rule = this->get_rule(prev_token->kind)->infix;
        (this->*infix_rule)();
    }
}