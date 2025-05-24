// parser.hpp
#ifndef PARSER_HPP
#define PARSER_HPP

#include "D:\vs code\PROJECT\PBL_TRANSPILER\ver4\lexer.hpp"
#include "D:\vs code\PROJECT\PBL_TRANSPILER\ver4\ast.hpp"
#include <memory>

class Parser {
public:
    explicit Parser(Lexer& lexer);

    std::unique_ptr<ASTNode> parse();

private:
    Lexer& lexer;
    Token current;
    Token prev;

    void advance();
    bool match(TokenType type);
    bool check(TokenType type);
    bool expect(TokenType type, const std::string& errMsg);
    bool isAtEnd() ; // Returns true if current token is END_OF_FILE
    Token previous(); // Returns the previous token (if you keep track)
    // Top-level rules
    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<ASTNode> parseDeclaration();
    
    std::unique_ptr<ASTNode> parseType();

    // Declarations
    std::unique_ptr<ASTNode> parseFunctionDecl();
    std::unique_ptr<ASTNode> parseClassDecl();
    std::unique_ptr<ASTNode> parseStructDecl();
    std::unique_ptr<ASTNode> parseEnumDecl();
    std::unique_ptr<ASTNode> parseUnionDecl();
    std::unique_ptr<ASTNode> parseNamespaceDecl();
    std::unique_ptr<ASTNode> parseVariableDecl();
    std::unique_ptr<ASTNode> parseTypedefDecl();
    std::unique_ptr<ASTNode> parseUsingDirective();
    std::unique_ptr<ASTNode> parsePreprocessorDirective();

    // Statements
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseBlock();
    std::unique_ptr<ASTNode> parseIfStmt();
    std::unique_ptr<ASTNode> parseElseStmt();
    std::unique_ptr<ASTNode> parseSwitchStmt();
    std::unique_ptr<ASTNode> parseCaseStmt();
    std::unique_ptr<ASTNode> parseDefaultStmt();
    std::unique_ptr<ASTNode> parseWhileStmt();
    std::unique_ptr<ASTNode> parseDoWhileStmt();
    std::unique_ptr<ASTNode> parseForStmt();
    std::unique_ptr<ASTNode> parseReturnStmt();
    std::unique_ptr<ASTNode> parseBreakStmt();
    std::unique_ptr<ASTNode> parseContinueStmt();
    std::unique_ptr<ASTNode> parseGotoStmt();
    std::unique_ptr<ASTNode> parseTryStmt();
    std::unique_ptr<ASTNode> parseCatchStmt();
    std::unique_ptr<ASTNode> parseThrowStmt();

    // Expressions
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseTernary();
    std::unique_ptr<ASTNode> parseLogicalOr();
    std::unique_ptr<ASTNode> parseLogicalAnd();
    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseRelational();
    std::unique_ptr<ASTNode> parseAdditive();
    std::unique_ptr<ASTNode> parseMultiplicative();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parsePostfix();
    std::unique_ptr<ASTNode> parsePrimary();

    // Specialized constructs
    std::unique_ptr<ASTNode> parseStreamExpr();
    std::unique_ptr<ASTNode> parseFunctionCallSuffix(std::unique_ptr<ASTNode> callee);
    std::unique_ptr<ASTNode> parseTemplateTypeSuffix(std::string baseName);

    // Helpers
    bool isTypeToken(TokenType type);
    std::string tokenToString(TokenType type);
};

#endif // PARSER_HPP