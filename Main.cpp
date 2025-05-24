#include "D:\vs code\PROJECT\PBL_TRANSPILER\ver4\lexer.hpp"
#include "D:\vs code\PROJECT\PBL_TRANSPILER\ver4\lexer_tester.hpp"  // Include lexer tester header
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <source_file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open source file '" << argv[1] << "'\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    file.close();

    std::cout << "Running lexer tester on source file: " << argv[1] << "\n\n";

    // Call lexer tester function that prints tokens or errors
    testLexer(source);

    std::cout << "\nLexer test completed.\n";

    return 0;
}
