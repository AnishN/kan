#include "asm.hpp"
#include "defer.hpp"
#include "error.hpp"
#include "file.hpp"
#include "fmt.hpp"
#include "hash_map.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "rng.hpp"
#include "str.hpp"
#include "source.hpp"
#include "token.hpp"
#include "vector.hpp"

Error exec() {
    Error error = Error::Okay;
    
    String path;
    try_error(path.init_from_ptr("../examples/test.kan"));
    defer(path.fini());

    Source source;
    source.init_from_path(&path);
    defer(source.fini());
    fmt::printlnf("File: {}\n{}\n", path, source.str);
    
    Vector<Token> tokens;
    tokens.init();
    defer(tokens.fini());

    Lexer lexer;
    lexer.init(&source, &tokens);
    defer(lexer.fini());
    lexer.run();
    lexer.print_tokens();
    
    Parser parser;
    parser.init(&source, &tokens);
    defer(parser.fini());
    parser.run();

    return error;
}

int main() {
    Error error = exec();
    fmt::printlnf("Exit Code: {}", error);
    return (int)error;
}