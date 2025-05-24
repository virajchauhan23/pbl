#include "D:\vs code\PROJECT\PBL_TRANSPILER\ver4\lexer_tester.hpp"
#include "lexer.hpp"
#include <iostream>

void testLexer(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    // if (lexer.hasError()) {
    //     std::cerr << "Lexer error: " << lexer.getError() << std::endl;
    //     return;
    // }

    std::cout << "Tokens generated: " << tokens.size() << std::endl;
    for (const auto& token : tokens) {
        std::cout << token->toString() << std::endl;
    }
    // std::cout<<"foo"<<std::endl;
}
