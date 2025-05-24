#include "D:\vs code\PROJECT\PBL_TRANSPILER\ver4\parser.hpp"
#include <stdexcept>
#include <iostream>

// --- Constructor ---
Parser::Parser(Lexer& lexer)
    : lexer(lexer) {
    advance();
}

// --- Token helpers ---
void Parser::advance() {
    prev = current;
    auto tokenPtr = lexer.tokenize();
    if (!tokenPtr.empty()) {
        current = *tokenPtr.front();
    }
}

bool Parser::match(TokenType type) {
    if (current.type() == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    return current.type() == type;
}

bool Parser::isAtEnd() {
    return current.type() == TokenType::END_OF_FILE;
}

Token Parser::previous() {
    return prev;
}

bool Parser::expect(TokenType type, const std::string& errMsg) {
    if (current.type() == type) {
        advance();
        return true;
    }
    throw std::runtime_error(errMsg);
}

// --- Top-level parse ---
std::unique_ptr<ASTNode> Parser::parse() {
    return parseProgram();
}

std::unique_ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();
    while (current.type() != TokenType::END_OF_FILE) {
        auto decl = parseDeclaration();
        if (decl) program->globals.push_back(std::move(decl));
    }
    return program;
}

// --- Declarations ---
std::unique_ptr<ASTNode> Parser::parseDeclaration() {
    if (match(TokenType::CLASS)) return parseClassDecl();
    if (match(TokenType::STRUCT)) return parseStructDecl();
    if (match(TokenType::ENUM)) return parseEnumDecl();
    if (match(TokenType::UNION)) return parseUnionDecl();
    if (match(TokenType::NAMESPACE)) return parseNamespaceDecl();
    if (match(TokenType::TYPEDEF)) return parseTypedefDecl();
    if (match(TokenType::USING)) return parseUsingDirective();
    if (isTypeToken(current.type())) {
        // Could be variable or function
        Token lookahead = current;
        advance();
        if (current.type() == TokenType::IDENTIFIER) {
            Token nameToken = current;
            advance();
            if (current.type() == TokenType::LEFT_PAREN) {
                // Function
                return parseFunctionDecl();
            } else {
                // Variable
                return parseVariableDecl();
            }
        }
    }
    return parseStatement();
}

bool Parser::isTypeToken(TokenType type) {
    switch (type) {
        case TokenType::INT:
        case TokenType::FLOAT_TYPE:
        case TokenType::DOUBLE:
        case TokenType::CHAR:
        case TokenType::BOOL:
        case TokenType::VOID:
        case TokenType::IDENTIFIER: // for user-defined types
            return true;
        default:
            return false;
    }
}

// --- Example: Class Declaration ---

std::unique_ptr<ASTNode> Parser::parseClassDecl() {
    expect(TokenType::IDENTIFIER, "Expected class name");
    std::string name = previous().text(); // FIX: use previous().text()
    expect(TokenType::LEFT_BRACE, "Expected '{' after class name");
    auto classNode = std::make_unique<ClassDecl>(name);
    while (current.type() != TokenType::RIGHT_BRACE && current.type() != TokenType::END_OF_FILE) {
        if (match(TokenType::PUBLIC)) {
            expect(TokenType::COLON, "Expected ':' after 'public'");
            // Optionally store access specifier in AST
            continue;
        }
        if (match(TokenType::PRIVATE)) {
            expect(TokenType::COLON, "Expected ':' after 'private'");
            continue;
        }
        if (match(TokenType::PROTECTED)) {
            expect(TokenType::COLON, "Expected ':' after 'protected'");
            continue;
        }
        classNode->members.push_back(parseDeclaration());
    }
    expect(TokenType::RIGHT_BRACE, "Expected '}' after class body");
    return classNode;
}
std::unique_ptr<ASTNode> Parser::parseStructDecl() {
    expect(TokenType::STRUCT, "Expected 'struct'");
    expect(TokenType::IDENTIFIER, "Expected struct name");
    std::string name = previous().text(); // FIX: use previous().text()
    expect(TokenType::LEFT_BRACE, "Expected '{' after struct name");
    auto structNode = std::make_unique<StructDecl>(name);

    while (current.type() != TokenType::RIGHT_BRACE && current.type() != TokenType::END_OF_FILE) {
        // Handle access specifiers if needed
        if (match(TokenType::PUBLIC)) {
            expect(TokenType::COLON, "Expected ':' after 'public'");
            continue;
        }
        if (match(TokenType::PRIVATE)) {
            expect(TokenType::COLON, "Expected ':' after 'private'");
            continue;
        }
        if (match(TokenType::PROTECTED)) {
            expect(TokenType::COLON, "Expected ':' after 'protected'");
            continue;
        }
        structNode->members.push_back(parseDeclaration());
    }
    expect(TokenType::RIGHT_BRACE, "Expected '}' after struct body");
    // Optionally expect(TokenType::SEMICOLON, "Expected ';' after struct declaration");
    return structNode;
}

// --- Example: Variable Declaration ---
std::unique_ptr<ASTNode> Parser::parseVariableDecl() {
    std::string typeName = current.text();
    advance();
    expect(TokenType::IDENTIFIER, "Expected variable name");
    std::string varName = previous().text(); // FIX: use previous().text()
    auto varNode = std::make_unique<VarDecl>(varName);
    varNode->type = std::make_unique<Identifier>(typeName);
    if (match(TokenType::EQUAL)) {
        varNode->initializer = parseExpression();
    }
    expect(TokenType::SEMICOLON, "Expected ';' after variable declaration");
    return varNode;
}

// --- Example: Function Declaration ---
std::unique_ptr<ASTNode> Parser::parseFunctionDecl() {
    std::string returnType = current.text();
    advance();
    expect(TokenType::IDENTIFIER, "Expected function name");
    std::string funcName = previous().text();
    expect(TokenType::LEFT_PAREN, "Expected '(' after function name");
    auto funcNode = std::make_unique<FunctionDecl>(funcName);
    funcNode->returnType = std::make_unique<Identifier>(returnType);
    // Parse parameters (not shown here)
    expect(TokenType::RIGHT_PAREN, "Expected ')' after parameters");
    funcNode->body = parseBlock();
    return funcNode;
}

// --- Example: Block ---
std::unique_ptr<ASTNode> Parser::parseBlock() {
    expect(TokenType::LEFT_BRACE, "Expected '{' to start block");
    auto block = std::make_unique<BlockStmt>();
    while (current.type() != TokenType::RIGHT_BRACE && current.type() != TokenType::END_OF_FILE) {
        block->statements.push_back(parseStatement());
    }
    expect(TokenType::RIGHT_BRACE, "Expected '}' to end block");
    return block;
}

// --- Example: Statement ---
std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (match(TokenType::IF)) return parseIfStmt();
    if (match(TokenType::WHILE)) return parseWhileStmt();
    if (match(TokenType::FOR)) return parseForStmt();
    if (match(TokenType::RETURN)) return parseReturnStmt();
    if (current.type() == TokenType::LEFT_BRACE) return parseBlock();
    // Fallback: expression statement
    auto expr = parseExpression();
    expect(TokenType::SEMICOLON, "Expected ';' after expression");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

// --- Example: Expression (expand as needed) ---
// std::unique_ptr<ASTNode> Parser::parseExpression() {
//     // For now, just parse a primary (expand with precedence climbing)
//     return parsePrimary();
// }

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    // C++ casts
    if (match(TokenType::STATIC_CAST)) {
        expect(TokenType::LESS, "Expected '<' after static_cast");
        auto type = parseType();
        expect(TokenType::GREATER, "Expected '>' after type");
        expect(TokenType::LEFT_PAREN, "Expected '(' after '>'");
        auto expr = parseExpression();
        expect(TokenType::RIGHT_PAREN, "Expected ')'");
        return std::make_unique<StaticCastExpr>(std::move(type), std::move(expr));
    }
    if (match(TokenType::DYNAMIC_CAST)) {
        expect(TokenType::LESS, "Expected '<' after dynamic_cast");
        auto type = parseType();
        expect(TokenType::GREATER, "Expected '>' after type");
        expect(TokenType::LEFT_PAREN, "Expected '(' after '>'");
        auto expr = parseExpression();
        expect(TokenType::RIGHT_PAREN, "Expected ')'");
        return std::make_unique<DynamicCastExpr>(std::move(type), std::move(expr));
    }
    if (match(TokenType::CONST_CAST)) {
        expect(TokenType::LESS, "Expected '<' after const_cast");
        auto type = parseType();
        expect(TokenType::GREATER, "Expected '>' after type");
        expect(TokenType::LEFT_PAREN, "Expected '(' after '>'");
        auto expr = parseExpression();
        expect(TokenType::RIGHT_PAREN, "Expected ')'");
        return std::make_unique<ConstCastExpr>(std::move(type), std::move(expr));
    }
    if (match(TokenType::REINTERPRET_CAST)) {
        expect(TokenType::LESS, "Expected '<' after reinterpret_cast");
        auto type = parseType();
        expect(TokenType::GREATER, "Expected '>' after type");
        expect(TokenType::LEFT_PAREN, "Expected '(' after '>'");
        auto expr = parseExpression();
        expect(TokenType::RIGHT_PAREN, "Expected ')'");
        return std::make_unique<ReinterpretCastExpr>(std::move(type), std::move(expr));
    }

    // new/delete
    if (match(TokenType::NEW)) {
        auto type = parseType();
        std::vector<std::unique_ptr<ASTNode>> args;
        if (match(TokenType::LEFT_PAREN)) {
            if (!check(TokenType::RIGHT_PAREN)) {
                do {
                    args.push_back(parseExpression());
                } while (match(TokenType::COMMA));
            }
            expect(TokenType::RIGHT_PAREN, "Expected ')' after new arguments");
        }
        return std::make_unique<NewExpr>(std::move(type), std::move(args));
    }
    if (match(TokenType::DELETE)) {
        auto expr = parseExpression();
        return std::make_unique<DeleteExpr>(std::move(expr));
    }

    // Lambda
    if (match(TokenType::LEFT_BRACKET)) {
        // Parse capture list (skip for now)
        while (!match(TokenType::RIGHT_BRACKET)) advance();
        expect(TokenType::LEFT_PAREN, "Expected '(' after lambda capture");
        std::vector<std::unique_ptr<ASTNode>> params;
        if (!check(TokenType::RIGHT_PAREN)) {
            do {
                auto type = parseType();
                expect(TokenType::IDENTIFIER, "Expected parameter name");
                std::string name = previous().text();
                params.push_back(std::make_unique<VarDecl>(name, std::move(type)));
            } while (match(TokenType::COMMA));
        }
        expect(TokenType::RIGHT_PAREN, "Expected ')' after lambda params");
        auto body = parseBlock();
        return std::make_unique<LambdaExpr>(std::move(params), std::move(body));
    }

    // Literals
    if (match(TokenType::INTEGER)) {
        return std::make_unique<Literal>(current.text(), "int");
    }
    if (match(TokenType::FLOAT)) {
        return std::make_unique<Literal>(current.text(), "float");
    }
    if (match(TokenType::STRING)) {
        return std::make_unique<Literal>(current.text(), "string");
    }
    if (match(TokenType::CHARACTER)) {
        return std::make_unique<Literal>(current.text(), "char");
    }

    // Identifier
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<Identifier>(current.text());
    }

    // Parenthesized expression
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = parseExpression();
        expect(TokenType::RIGHT_PAREN, "Expected ')'");
        return expr;
    }

    throw std::runtime_error("Unexpected token in expression");
}

// --- parseCatchStmt ---
std::unique_ptr<ASTNode> Parser::parseCatchStmt() {
    expect(TokenType::CATCH, "Expected 'catch'");
    expect(TokenType::LEFT_PAREN, "Expected '(' after 'catch'");
    auto exceptionType = parseType(); // FIXED: use auto, not std::string
    expect(TokenType::IDENTIFIER, "Expected exception variable name");
    std::string name = previous().text();
    expect(TokenType::RIGHT_PAREN, "Expected ')' after catch parameter");
    auto body = parseBlock();
    return std::make_unique<CatchStmt>(std::move(exceptionType), name, std::move(body));
}

// --- parseTryStmt ---
std::unique_ptr<ASTNode> Parser::parseTryStmt() {
    expect(TokenType::TRY, "Expected 'try'");
    auto tryBlock = parseBlock();
    std::vector<std::unique_ptr<ASTNode>> catches;
    while (check(TokenType::CATCH)) {
        catches.push_back(parseCatchStmt());
    }
    return std::make_unique<TryStmt>(std::move(tryBlock), std::move(catches));
}

// --- parseThrowStmt ---
std::unique_ptr<ASTNode> Parser::parseThrowStmt() {
    expect(TokenType::THROW, "Expected 'throw'");
    auto expr = parseExpression();
    expect(TokenType::SEMICOLON, "Expected ';' after throw statement");
    return std::make_unique<ThrowStmt>(std::move(expr));
}

// --- parseBreakStmt ---
std::unique_ptr<ASTNode> Parser::parseBreakStmt() {
    expect(TokenType::BREAK, "Expected 'break'");
    expect(TokenType::SEMICOLON, "Expected ';' after break");
    return std::make_unique<BreakStmt>();
}

// --- parseContinueStmt ---
std::unique_ptr<ASTNode> Parser::parseContinueStmt() {
    expect(TokenType::CONTINUE, "Expected 'continue'");
    expect(TokenType::SEMICOLON, "Expected ';' after continue");
    return std::make_unique<ContinueStmt>();
}

// --- parseGotoStmt ---
std::unique_ptr<ASTNode> Parser::parseGotoStmt() {
    expect(TokenType::GOTO, "Expected 'goto'");
    expect(TokenType::IDENTIFIER, "Expected label after 'goto'");
    std::string name = previous().text();
    expect(TokenType::SEMICOLON, "Expected ';' after goto statement");
    return std::make_unique<GotoStmt>(name); 
}

// --- parseElseStmt ---
std::unique_ptr<ASTNode> Parser::parseElseStmt() {
    expect(TokenType::ELSE, "Expected 'else'");
    auto elseBranch = parseStatement();
    return std::make_unique<ElseStmt>(std::move(elseBranch));
}

// --- parseSwitchStmt ---
std::unique_ptr<ASTNode> Parser::parseSwitchStmt() {
    expect(TokenType::SWITCH, "Expected 'switch'");
    expect(TokenType::LEFT_PAREN, "Expected '(' after 'switch'");
    auto condition = parseExpression();
    expect(TokenType::RIGHT_PAREN, "Expected ')' after switch condition");
    expect(TokenType::LEFT_BRACE, "Expected '{' after switch");
    std::vector<std::unique_ptr<ASTNode>> cases;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (check(TokenType::CASE)) {
            cases.push_back(parseCaseStmt());
        } else if (check(TokenType::DEFAULT)) {
            cases.push_back(parseDefaultStmt());
        } else {
            cases.push_back(parseStatement());
        }
    }
    expect(TokenType::RIGHT_BRACE, "Expected '}' after switch body");
    return std::make_unique<SwitchStmt>(std::move(condition), std::move(cases));
}

// --- parseCaseStmt ---
std::unique_ptr<ASTNode> Parser::parseCaseStmt() {
    expect(TokenType::CASE, "Expected 'case'");
    auto value = parseExpression();
    expect(TokenType::COLON, "Expected ':' after case value");
    std::vector<std::unique_ptr<ASTNode>> statements;
    while (!check(TokenType::CASE) && !check(TokenType::DEFAULT) && !check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(parseStatement());
    }
    return std::make_unique<CaseStmt>(std::move(value), std::move(statements));
}

// --- parseDefaultStmt ---
std::unique_ptr<ASTNode> Parser::parseDefaultStmt() {
    expect(TokenType::DEFAULT, "Expected 'default'");
    expect(TokenType::COLON, "Expected ':' after default");
    std::vector<std::unique_ptr<ASTNode>> statements;
    while (!check(TokenType::CASE) && !check(TokenType::DEFAULT) && !check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(parseStatement());
    }
    return std::make_unique<DefaultStmt>(std::move(statements));
}

// --- parseDoWhileStmt ---
std::unique_ptr<ASTNode> Parser::parseDoWhileStmt() {
    expect(TokenType::DO, "Expected 'do'");
    auto body = parseStatement();
    expect(TokenType::WHILE, "Expected 'while' after do body");
    expect(TokenType::LEFT_PAREN, "Expected '(' after 'while'");
    auto condition = parseExpression();
    expect(TokenType::RIGHT_PAREN, "Expected ')' after do-while condition");
    expect(TokenType::SEMICOLON, "Expected ';' after do-while");
    return std::make_unique<DoWhileStmt>(std::move(condition), std::move(body));
}

// --- parseUnionDecl ---
std::unique_ptr<ASTNode> Parser::parseUnionDecl() {
    expect(TokenType::UNION, "Expected 'union'");
    // if (!check(TokenType::IDENTIFIER)) error("Expected union name");
    // std::string unionName = advance()->lexeme;
    expect(TokenType::IDENTIFIER, "Expected union name");
    std::string name = previous().text();

    expect(TokenType::LEFT_BRACE, "Expected '{' after union name");
    std::vector<std::unique_ptr<ASTNode>> members;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        members.push_back(parseVariableDecl());
        expect(TokenType::SEMICOLON, "Expected ';' after union member");
    }
    expect(TokenType::RIGHT_BRACE, "Expected '}' after union body");
    expect(TokenType::SEMICOLON, "Expected ';' after union declaration");
   return std::make_unique<UnionDecl>(name, std::move(members));
}

// --- parseTypedefDecl ---
std::unique_ptr<ASTNode> Parser::parseTypedefDecl() {
    expect(TokenType::TYPEDEF, "Expected 'typedef'");
    auto aliasedType = parseType(); 
    expect(TokenType::IDENTIFIER, "Expected typedef alias name");
    std::string name = previous().text();
    expect(TokenType::SEMICOLON, "Expected ';' after typedef");
    return std::make_unique<TypedefDecl>(name, std::move(aliasedType));
}

// --- parseTemplateTypeSuffix ---
std::unique_ptr<ASTNode> Parser::parseTemplateTypeSuffix(std::string baseName) {
    expect(TokenType::LESS, "Expected '<' for template type");
    std::vector<std::unique_ptr<ASTNode>> typeArgs;
    do {
        typeArgs.push_back(parseType());
    } while (match(TokenType::COMMA));
    expect(TokenType::GREATER, "Expected '>' after template arguments");
    return std::make_unique<TemplateType>(baseName, std::move(typeArgs));
}

// --- parseFunctionCallSuffix ---
std::unique_ptr<ASTNode> Parser::parseFunctionCallSuffix(std::unique_ptr<ASTNode> callee) {
    expect(TokenType::LEFT_PAREN, "Expected '(' after function name");
    std::vector<std::unique_ptr<ASTNode>> args;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            args.push_back(parseExpression());
        } while (match(TokenType::COMMA));
    }
    expect(TokenType::RIGHT_PAREN, "Expected ')' after arguments");
    return std::make_unique<FunctionCall>(std::move(callee), std::move(args));
}

// --- parseStreamExpr ---
std::unique_ptr<ASTNode> Parser::parseStreamExpr() {
    // Example: cout << x << y;
    auto stream = parsePrimary();
    while (match(TokenType::LESS_LESS) || match(TokenType::GREATER_GREATER)) {
        Token op = previous();
        auto right = parseExpression();
        stream = std::make_unique<StreamExpr>(std::move(stream), op.text(), std::move(right));
    }
    return stream;
}
std::unique_ptr<ASTNode> Parser::parseWhileStmt() {
    expect(TokenType::WHILE, "Expected 'while'");
    expect(TokenType::LEFT_PAREN, "Expected '(' after 'while'");
    auto condition = parseExpression();
    expect(TokenType::RIGHT_PAREN, "Expected ')' after condition");
    auto body = parseStatement();
    // Assuming WhileStmt(condition, body)
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseForStmt() {
    expect(TokenType::FOR, "Expected 'for'");
    expect(TokenType::LEFT_PAREN, "Expected '(' after 'for'");
    std::unique_ptr<ASTNode> init = nullptr;
    if (!match(TokenType::SEMICOLON))
        init = parseExpression();
    expect(TokenType::SEMICOLON, "Expected ';' after for-init");
    auto condition = parseExpression();
    expect(TokenType::SEMICOLON, "Expected ';' after for-condition");
    auto increment = parseExpression();
    expect(TokenType::RIGHT_PAREN, "Expected ')' after for-increment");
    auto body = parseStatement();
    // Assuming ForStmt(init, condition, increment, body)
    return std::make_unique<ForStmt>(std::move(init), std::move(condition), std::move(increment), std::move(body));
}
std::unique_ptr<ASTNode> Parser::parseIfStmt() {
    expect(TokenType::IF, "Expected 'if'");
    expect(TokenType::LEFT_PAREN, "Expected '(' after 'if'");
    auto condition = parseExpression();
    expect(TokenType::RIGHT_PAREN, "Expected ')' after condition");
    auto thenBranch = parseStatement();
    std::unique_ptr<ASTNode> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        elseBranch = parseStatement();
    }
    // Assuming IfStmt(condition, thenBranch, elseBranch)
    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<ASTNode> Parser::parseReturnStmt() {
    expect(TokenType::RETURN, "Expected 'return'");
    std::unique_ptr<ASTNode> expr = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        expr = parseExpression();
    }
    expect(TokenType::SEMICOLON, "Expected ';' after return");
    return std::make_unique<ReturnStmt>(std::move(expr));
}



std::unique_ptr<ASTNode> Parser::parseNamespaceDecl() {
    expect(TokenType::NAMESPACE, "Expected 'namespace'");
    expect(TokenType::IDENTIFIER, "Expected namespace name");
    std::string name = previous().text(); // FIX: use previous().text()
    expect(TokenType::LEFT_BRACE, "Expected '{' after namespace name");
    auto nsNode = std::make_unique<NamespaceDecl>(name);
    while (current.type() != TokenType::RIGHT_BRACE && current.type() != TokenType::END_OF_FILE) {
        nsNode->declarations.push_back(parseDeclaration());
    }
    expect(TokenType::RIGHT_BRACE, "Expected '}' after namespace body");
    return nsNode;
}

std::unique_ptr<ASTNode> Parser::parseUsingDirective() {
    expect(TokenType::USING, "Expected 'using'");
    expect(TokenType::IDENTIFIER, "Expected identifier after 'using'");
    std::string name = previous().text(); // FIX: use previous().text()
    expect(TokenType::SEMICOLON, "Expected ';' after using directive");
    return std::make_unique<UsingDirective>(name);
}

// --- Expression Parsing with Precedence ---
std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseTernary();
}

std::unique_ptr<ASTNode> Parser::parseTernary() {
    auto cond = parseLogicalOr();
    if (match(TokenType::QUESTION)) {
        auto thenExpr = parseExpression();
        expect(TokenType::COLON, "Expected ':' in ternary expression");
        auto elseExpr = parseExpression();
        return std::make_unique<TernaryExpr>(std::move(cond), std::move(thenExpr), std::move(elseExpr));
    }
    return cond;
}

std::unique_ptr<ASTNode> Parser::parseLogicalOr() {
    auto left = parseLogicalAnd();
    while (match(TokenType::OR_OR)) {
        auto right = parseLogicalAnd();
        left = std::make_unique<BinaryExpr>("||", std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseLogicalAnd() {
    auto left = parseEquality();
    while (match(TokenType::AND_AND)) {
        auto right = parseEquality();
        left = std::make_unique<BinaryExpr>("&&", std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseEquality() {
    auto left = parseRelational();
    while (match(TokenType::EQUAL_EQUAL) || match(TokenType::NOT_EQUAL)) {
        std::string op = previous().text();
        auto right = parseRelational();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseRelational() {
    auto left = parseAdditive();
    while (match(TokenType::LESS) || match(TokenType::LESS_EQUAL) ||
           match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL)) {
        std::string op = previous().text();
        auto right = parseAdditive();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseAdditive() {
    auto left = parseMultiplicative();
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        std::string op = previous().text();
        auto right = parseMultiplicative();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseMultiplicative() {
    auto left = parseUnary();
    while (match(TokenType::STAR) || match(TokenType::SLASH) || match(TokenType::PERCENT)) {
        std::string op = previous().text();
        auto right = parseUnary();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
    if (match(TokenType::EXCLAIM) || match(TokenType::MINUS) || match(TokenType::INCREMENT) || match(TokenType::DECREMENT)) {
        std::string op = previous().text();
        auto right = parseUnary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }
    return parsePostfix();
}

std::unique_ptr<ASTNode> Parser::parsePostfix() {
    auto expr = parsePrimary();
    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = parseFunctionCallSuffix(std::move(expr));
        } else if (match(TokenType::LEFT_BRACKET)) {
            auto index = parseExpression();
            expect(TokenType::RIGHT_BRACKET, "Expected ']' after array index");
            expr = std::make_unique<ArrayAccess>(std::move(expr), std::move(index));
        } else if (match(TokenType::DOT) || match(TokenType::ARROW)) {
            std::string memberOp = previous().text();
            expect(TokenType::IDENTIFIER, "Expected member name after '.' or '->'");
            std::string member = previous().text();
            expr = std::make_unique<MemberAccess>(std::move(expr), member, memberOp == "->");
        } else if (match(TokenType::SCOPE)) {
            expect(TokenType::IDENTIFIER, "Expected identifier after '::'");
            std::string name = current.text();
            advance();
            expr = std::make_unique<QualifiedName>(std::move(expr), name);
        } else {
            break;
        }
    }
    return expr;
}


std::unique_ptr<ASTNode> Parser::parseType() {
    expect(TokenType::IDENTIFIER, "Expected type name");
    std::string base = previous().text(); // FIX: use previous().text()
    if (match(TokenType::LESS)) {
        std::vector<std::unique_ptr<ASTNode>> typeArgs;
        do {
            typeArgs.push_back(parseType());
        } while (match(TokenType::COMMA));
        expect(TokenType::GREATER, "Expected '>' after template arguments");
        return std::make_unique<TemplateType>(base, std::move(typeArgs));
    }
    return std::make_unique<Identifier>(base);
}


std::unique_ptr<ASTNode> Parser::parsePreprocessorDirective() {
    if (!match(TokenType::HASH)) return nullptr;

    if (match(TokenType::PREPROCESSOR_INCLUDE)) {
        expect(TokenType::STRING, "Expected header after #include");
        std::string header = previous().text();
        return std::make_unique<PreprocessorInclude>(header);
    }
    if (match(TokenType::PREPROCESSOR_DEFINE)) {
        expect(TokenType::IDENTIFIER, "Expected macro name after #define");
        std::string macro = previous().text();
        std::string value;
        // Optionally parse the macro value (until end of line)
        if (!check(TokenType::NEWLINE) && !isAtEnd()) {
            value = current.text();
            advance();
        }
        return std::make_unique<PreprocessorDefine>(macro, value);
    }
    if (match(TokenType::PREPROCESSOR_UNDEF)) {
        expect(TokenType::IDENTIFIER, "Expected macro name after #undef");
        std::string macro = previous().text();
        return std::make_unique<PreprocessorUndef>(macro);
    }
    if (match(TokenType::PREPROCESSOR_IFDEF)) {
        expect(TokenType::IDENTIFIER, "Expected macro name after #ifdef");
        std::string macro = previous().text();
        return std::make_unique<PreprocessorIfdef>(macro);
    }
    if (match(TokenType::PREPROCESSOR_IFNDEF)) {
        expect(TokenType::IDENTIFIER, "Expected macro name after #ifndef");
        std::string macro = previous().text();
        return std::make_unique<PreprocessorIfndef>(macro);
    }
    if (match(TokenType::PREPROCESSOR_IF)) {
        // Optionally parse the condition as a string or expression
        std::string condition = current.text();
        advance();
        return std::make_unique<PreprocessorIf>(condition);
    }
    if (match(TokenType::PREPROCESSOR_ELSE)) {
        return std::make_unique<PreprocessorElse>();
    }
    if (match(TokenType::PREPROCESSOR_ELIF)) {
        std::string condition = current.text();
        advance();
        return std::make_unique<PreprocessorElif>(condition);
    }
    if (match(TokenType::PREPROCESSOR_ENDIF)) {
        return std::make_unique<PreprocessorEndif>();
    }
    if (match(TokenType::PREPROCESSOR_PRAGMA)) {
        std::string pragma = current.text();
        advance();
        return std::make_unique<PreprocessorPragma>(pragma);
    }

    // Unknown or unsupported directive
    std::string unknown = current.text();
    advance();
    return std::make_unique<PreprocessorUnknown>(unknown);
}