#include "D:\\vs code\\PROJECT\\PBL_TRANSPILER\\ver4\\tokens.hpp"
#include <sstream>
#include <iomanip>
#include <unordered_map>

namespace {

static const std::unordered_map<TokenType, const char*> token_type_names = {
    {TokenType::END_OF_FILE, "END_OF_FILE"}, {TokenType::ERROR, "ERROR"},
    {TokenType::IDENTIFIER, "IDENTIFIER"}, {TokenType::INTEGER, "INTEGER"},
    {TokenType::FLOAT, "FLOAT"}, {TokenType::STRING, "STRING"},
    {TokenType::CHARACTER, "CHARACTER"}, {TokenType::INT, "INT"},
    {TokenType::VOID, "VOID"}, {TokenType::CHAR, "CHAR"},
    {TokenType::FLOAT_TYPE, "FLOAT_TYPE"}, {TokenType::DOUBLE, "DOUBLE"},
    {TokenType::BOOL, "BOOL"}, {TokenType::CLASS, "CLASS"},
    {TokenType::STRUCT, "STRUCT"}, {TokenType::ENUM, "ENUM"},
    {TokenType::UNION, "UNION"}, {TokenType::CONST, "CONST"},
    {TokenType::UNSIGNED, "UNSIGNED"}, {TokenType::SIGNED, "SIGNED"},
    {TokenType::SHORT, "SHORT"}, {TokenType::LONG, "LONG"},
    {TokenType::STATIC, "STATIC"}, {TokenType::EXTERN, "EXTERN"},
    {TokenType::REGISTER, "REGISTER"}, {TokenType::INLINE, "INLINE"},
    {TokenType::VIRTUAL, "VIRTUAL"}, {TokenType::EXPLICIT, "EXPLICIT"},
    {TokenType::FRIEND, "FRIEND"}, {TokenType::PRIVATE, "PRIVATE"},
    {TokenType::PUBLIC, "PUBLIC"}, {TokenType::PROTECTED, "PROTECTED"},
    {TokenType::IF, "IF"}, {TokenType::ELSE, "ELSE"},
    {TokenType::FOR, "FOR"}, {TokenType::WHILE, "WHILE"},
    {TokenType::DO, "DO"}, {TokenType::SWITCH, "SWITCH"},
    {TokenType::CASE, "CASE"}, {TokenType::DEFAULT, "DEFAULT"},
    {TokenType::BREAK, "BREAK"}, {TokenType::CONTINUE, "CONTINUE"},
    {TokenType::RETURN, "RETURN"}, {TokenType::GOTO, "GOTO"},
    {TokenType::NEWLINE, "NEWLINE"}, 

    {TokenType::PLUS, "PLUS"}, {TokenType::MINUS, "MINUS"},
    {TokenType::STAR, "STAR"}, {TokenType::SLASH, "SLASH"},
    {TokenType::PERCENT, "PERCENT"}, {TokenType::AMPERSAND, "AMPERSAND"},
    {TokenType::PIPE, "PIPE"}, {TokenType::CARET, "CARET"},
    {TokenType::TILDE, "TILDE"}, {TokenType::EXCLAIM, "EXCLAIM"},
    {TokenType::EQUAL, "EQUAL"}, {TokenType::LESS, "LESS"},
    {TokenType::GREATER, "GREATER"}, {TokenType::PLUS_EQUAL, "PLUS_EQUAL"},
    {TokenType::MINUS_EQUAL, "MINUS_EQUAL"}, {TokenType::STAR_EQUAL, "STAR_EQUAL"},
    {TokenType::SLASH_EQUAL, "SLASH_EQUAL"}, {TokenType::PERCENT_EQUAL, "PERCENT_EQUAL"},
    {TokenType::AND_EQUAL, "AND_EQUAL"}, {TokenType::OR_EQUAL, "OR_EQUAL"},
    {TokenType::XOR_EQUAL, "XOR_EQUAL"}, {TokenType::LESS_LESS, "LESS_LESS"},
    {TokenType::GREATER_GREATER, "GREATER_GREATER"},

    // New multi-char assignment operators
    {TokenType::LEFT_SHIFT_EQUAL, "LEFT_SHIFT_EQUAL"},
    {TokenType::RIGHT_SHIFT_EQUAL, "RIGHT_SHIFT_EQUAL"},

    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::NOT_EQUAL, "NOT_EQUAL"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::AND_AND, "AND_AND"},
    {TokenType::OR_OR, "OR_OR"},
    {TokenType::DECREMENT, "DECREMENT"},
    {TokenType::ARROW, "ARROW"},
    {TokenType::INCREMENT, "INCREMENT"},
   

    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::LEFT_BRACKET, "LEFT_BRACKET"},
    {TokenType::RIGHT_BRACKET, "RIGHT_BRACKET"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::DOT, "DOT"},
    {TokenType::COLON, "COLON"},
    {TokenType::SCOPE, "SCOPE"},
    {TokenType::HASH, "HASH"},
    {TokenType::QUESTION, "QUESTION"},

    // {TokenType::STRING_LITERAL, "STRING_LITERAL"},
    // {TokenType::CHAR_LITERAL, "CHAR_LITERAL"},

    // STL Containers
    {TokenType::VECTOR, "VECTOR"},
    {TokenType::MAP, "MAP"},
    {TokenType::SET, "SET"},
    {TokenType::LIST, "LIST"},
    {TokenType::DEQUE, "DEQUE"},
    {TokenType::UNORDERED_MAP, "UNORDERED_MAP"},
    {TokenType::UNORDERED_SET, "UNORDERED_SET"},
    {TokenType::STRING_LIB, "STRING_LIB"},
    {TokenType::MULTIMAP, "MULTIMAP"},
    {TokenType::MULTISET, "MULTISET"},
    {TokenType::STACK, "STACK"},
    {TokenType::QUEUE, "QUEUE"},
    {TokenType::PRIORITY_QUEUE, "PRIORITY_QUEUE"},
    {TokenType::BITSET, "BITSET"},
    {TokenType::ARRAY, "ARRAY"},
    {TokenType::FORWARD_LIST, "FORWARD_LIST"},
    {TokenType::PAIR, "PAIR"},
    {TokenType::TUPLE, "TUPLE"},
    {TokenType::OPTIONAL, "OPTIONAL"},
    {TokenType::VARIANT, "VARIANT"},
    {TokenType::ANY, "ANY"},
    {TokenType::SPAN, "SPAN"},
    {TokenType::VALARRAY, "VALARRAY"},


    // C standard lib functions
    {TokenType::PRINTF, "PRINTF"},
    {TokenType::SCANF, "SCANF"},
    {TokenType::MALLOC, "MALLOC"},
    {TokenType::FREE, "FREE"},
    {TokenType::MEMCPY, "MEMCPY"},
    {TokenType::STRCPY, "STRCPY"},
    {TokenType::STRLEN, "STRLEN"},

    // Template and Namespace tokens
    {TokenType::TEMPLATE, "TEMPLATE"},
    {TokenType::NAMESPACE, "NAMESPACE"},
    {TokenType::USING, "USING"},
    {TokenType::TYPEDEF, "TYPEDEF"},
    {TokenType::TEMPLATE_LESS, "TEMPLATE_LESS"},
    {TokenType::TEMPLATE_GREATER, "TEMPLATE_GREATER"},
    {TokenType::TEMPLATE_COMMA, "TEMPLATE_COMMA"},

    // Preprocessor directives
    {TokenType::PREPROCESSOR_INCLUDE, "PREPROCESSOR_INCLUDE"},
    {TokenType::PREPROCESSOR_DEFINE, "PREPROCESSOR_DEFINE"},
    {TokenType::PREPROCESSOR_UNDEF, "PREPROCESSOR_UNDEF"},
    {TokenType::PREPROCESSOR_PRAGMA, "PREPROCESSOR_PRAGMA"},
    {TokenType::PREPROCESSOR_UNKNOWN, "PREPROCESSOR_UNKNOWN"},
    {TokenType::PREPROCESSOR_IFDEF, "PREPROCESSOR_IFDEF"},
    {TokenType::PREPROCESSOR_IFNDEF, "PREPROCESSOR_IFNDEF"},
    {TokenType::PREPROCESSOR_IF, "PREPROCESSOR_IF"},
    {TokenType::PREPROCESSOR_ELIF, "PREPROCESSOR_ELIF"},
    {TokenType::PREPROCESSOR_ELSE, "PREPROCESSOR_ELSE"},
    {TokenType::PREPROCESSOR_ENDIF, "PREPROCESSOR_ENDIF"}
};

const char* getTokenTypeName(TokenType type) {
    auto it = token_type_names.find(type);
    return it != token_type_names.end() ? it->second : "UNKNOWN";
}

} // unnamed namespace

std::string Token::toString() const {
    std::ostringstream oss;
    oss << "Token(type=" << getTokenTypeName(type())
        << ", text='" << text() << "'"
        << ", line=" << line()
        << ", column=" << column();

    if (type() == TokenType::INTEGER) {
        oss << ", int_value=" << int_value();
    } else if (type() == TokenType::FLOAT) {
        oss << ", float_value=" << std::fixed << std::setprecision(6) << float_value();
    }

    oss << ")";
    return oss.str();
}
