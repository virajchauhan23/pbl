#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <memory>
// #include <optional>
#include "tokens.hpp"  // your existing token types for reference if needed

// Enumerate all node types in AST
enum class ASTNodeType {
    PROGRAM,
    PREPROCESSOR_DIRECTIVE,
    NAMESPACE_DECL,
    USING_DIRECTIVE,
    
    CLASS_DECL,
    STRUCT_DECL,
    ENUM_DECL,
    UNION_DECL,
    
    FUNCTION_DECL,
    VAR_DECL,
    TYPEDEF_DECL,
    
    IF_STMT,
    ELSE_STMT,  // if you want explicit else node
    SWITCH_STMT,
    CASE_STMT,
    DEFAULT_STMT,
    FOR_STMT,
    WHILE_STMT,
    DO_WHILE_STMT,
    RETURN_STMT,
    BREAK_STMT,
    CONTINUE_STMT,
    GOTO_STMT,
    TRY_STMT,
    CATCH_STMT,
    THROW_STMT,
    
    BLOCK_STMT,
    EXPRESSION_STMT,
    
    BINARY_EXPR,
    UNARY_EXPR,
    TERNARY_EXPR,
    
    FUNCTION_CALL,
    MEMBER_ACCESS,
    ARRAY_ACCESS,
    VECTOR_ACCESS,
    
    LITERAL,
    IDENTIFIER,
    

    // Namespace and Template tokens
    TEMPLATE_CLASS_DECL,
    TEMPLATE_TYPE,
    POINTER_TYPE,
    REFERENCE_TYPE,
    QUALIFIED_TYPE,  // e.g., std::vector
    QUALIFIED_NAME,
    TEMPLATE_KEYWORD,
    USING_KEYWORD,
    NAMESPACE_KEYWORD,
    TEMPLATE_LESS,
    TEMPLATE_GREATER,
    TEMPLATE_COMMA,
    TEMPLATE_FUNCTION_DECL,
    // Template parameters and arguments
    TEMPLATE_PARAM,
    TEMPLATE_ARG,
    
    TYPE,  // generic type node
    
    LAMBDA_EXPR,
    
    // Special nodes for casts and RTTI
    STATIC_CAST_EXPR,
    DYNAMIC_CAST_EXPR,
    CONST_CAST_EXPR,
    REINTERPRET_CAST_EXPR,
    TYPEID_EXPR,
    
    // Stream Expressions
    STREAM_EXPR,  // for cout << x;
    
    
    
    // Preprocessor
    PREPROCESSOR_INCLUDE,
    PREPROCESSOR_DEFINE,
    PREPROCESSOR_IFDEF,
    PREPROCESSOR_IFNDEF,
    PREPROCESSOR_IF,
    PREPROCESSOR_ELIF,
    PREPROCESSOR_ELSE,
    PREPROCESSOR_ENDIF,
    PREPROCESSOR_UNDEF,
    PREPROCESSOR_PRAGMA,
    PREPROCESSOR_UNKNOWN,
    
    // Concurrency
    THREAD_DECL,
    MUTEX_DECL,
    LOCK_GUARD_DECL,
    UNIQUE_LOCK_DECL,
    ASYNC_EXPR,
    FUTURE_EXPR,
    PROMISE_DECL,
    
    // Exception Classes
    EXCEPTION_CLASS,
    LOGIC_ERROR_CLASS,
    RUNTIME_ERROR_CLASS,
    
    // STL containers
    VECTOR_TYPE,
    MAP_TYPE,
    SET_TYPE,
    LIST_TYPE,
    DEQUE_TYPE,
    UNORDERED_MAP_TYPE,
    UNORDERED_SET_TYPE,
    MULTIMAP_TYPE,
    MULTISET_TYPE,
    STACK_TYPE,
    QUEUE_TYPE,
    PRIORITY_QUEUE_TYPE,
    BITSET_TYPE,
    ARRAY_TYPE,
    FORWARD_LIST_TYPE,
    PAIR_TYPE,
    TUPLE_TYPE,
    STRING_TYPE,
    OPTIONAL_TYPE,
    VARIANT_TYPE,
    ANY_TYPE,
    SPAN_TYPE,
    VALARRAY_TYPE,
    INITIALIZER_LIST_EXPR,

    // Std functions
    PRINTF_CALL,
    SCANF_CALL,
    MALLOC_CALL,
    FREE_CALL,
    MEMCPY_CALL,
    STRCPY_CALL,
    STRLEN_CALL,
    
    // I/O Streams
    CIN_EXPR,
    COUT_EXPR,
    CERR_EXPR,
    CLIN_EXPR,
    GETLINE_CALL,
    PUT_CALL,
    GET_CALL,
    FLUSH_CALL,
    OPEN_CALL,
    CLOSE_CALL,
    READ_CALL,
    WRITE_CALL,
    
    // Algorithms
    SORT_CALL,
    FIND_CALL,
    COUNT_CALL,
    COPY_CALL,
    REVERSE_CALL,
    ACCUMULATE_CALL,
    ALL_OF_CALL,
    ANY_OF_CALL,
    NONE_OF_CALL,
    LOWER_BOUND_CALL,
    UPPER_BOUND_CALL,
    
    // Math Functions
    ABS_CALL,
    FABS_CALL,
    POW_CALL,
    SQRT_CALL,
    SIN_CALL,
    COS_CALL,
    TAN_CALL,
    FLOOR_CALL,
    CEIL_CALL,
    ROUND_CALL,
    RAND_CALL,
    SRAND_CALL,
    EXIT_CALL,
    
    // String functions
    STOI_CALL,
    STOF_CALL,
    STOD_CALL,
    TO_STRING_CALL,
    STRCMP_CALL,
    STRNCMP_CALL,
    STRCHR_CALL,
    STRRCHR_CALL,
    STRSTR_CALL,
    STRCAT_CALL,
    STRNCAT_CALL,
    
    // Memory
    NEW_EXPR,
    DELETE_EXPR,
    ALLOCATE_CALL,
    DEALLOCATE_CALL,
    
    // Time functions
    TIME_CALL,
    CLOCK_CALL,
    DIFFTIME_CALL,
    STRFTIME_CALL,
    LOCALTIME_CALL,
    GMTIME_CALL,
    
    
    
    // Keywords and other tokens
    CONST_KEYWORD,
    STATIC_KEYWORD,
    EXTERN_KEYWORD,
    REGISTER_KEYWORD,
    INLINE_KEYWORD,
    VIRTUAL_KEYWORD,
    EXPLICIT_KEYWORD,
    FRIEND_KEYWORD,
    PRIVATE_KEYWORD,
    PUBLIC_KEYWORD,
    PROTECTED_KEYWORD,
    
    // Logical & control keywords
    IF_KEYWORD,
    ELSE_KEYWORD,
    FOR_KEYWORD,
    WHILE_KEYWORD,
    DO_KEYWORD,
    SWITCH_KEYWORD,
    CASE_KEYWORD,
    DEFAULT_KEYWORD,
    BREAK_KEYWORD,
    CONTINUE_KEYWORD,
    RETURN_KEYWORD,
    GOTO_KEYWORD,
    
    // Exception keywords
    TRY_KEYWORD,
    CATCH_KEYWORD,
    THROW_KEYWORD,
    
    // Cast keywords
    STATIC_CAST_KEYWORD,
    DYNAMIC_CAST_KEYWORD,
    CONST_CAST_KEYWORD,
    REINTERPRET_CAST_KEYWORD,
    
    // Additional nodes as needed
};

// Base ASTNode class
class ASTNode {
public:
    ASTNodeType type;
    explicit ASTNode(ASTNodeType t) : type(t) {}
    virtual ~ASTNode() = default;
};


// Program node: root container for all global declarations
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> globals;

    Program() : ASTNode(ASTNodeType::PROGRAM) {}
};


// Preprocessor directive base
class PreprocessorDirective : public ASTNode {
public:
    std::string directiveText;  // e.g., "#include <iostream>"

    explicit PreprocessorDirective(std::string text)
        : ASTNode(ASTNodeType::PREPROCESSOR_DIRECTIVE), directiveText(std::move(text)) {}
};


// Namespace declaration
class NamespaceDecl : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> declarations;

    explicit NamespaceDecl(std::string nsName)
        : ASTNode(ASTNodeType::NAMESPACE_DECL), name(std::move(nsName)) {}
};


// Using directive (e.g., using namespace std;)
class UsingDirective : public ASTNode {
public:
    std::string namespaceName;

    explicit UsingDirective(std::string ns)
        : ASTNode(ASTNodeType::USING_DIRECTIVE), namespaceName(std::move(ns)) {}
};


// Class declaration with members
class ClassDecl : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> members;  // variables, functions, nested classes, etc.

    explicit ClassDecl(std::string className)
        : ASTNode(ASTNodeType::CLASS_DECL), name(std::move(className)) {}
};


// Struct declaration, similar to class
class StructDecl : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> members;

    explicit StructDecl(std::string structName)
        : ASTNode(ASTNodeType::STRUCT_DECL), name(std::move(structName)) {}
};


// Enum declaration
class EnumDecl : public ASTNode {
public:
    std::string name;
    // Use -1 as a sentinel for "no explicit value"
    // Helper: if value == -1, no explicit value was set
    std::vector<std::pair<std::string, int>> enumerators;

    explicit EnumDecl(std::string enumName)
        : ASTNode(ASTNodeType::ENUM_DECL), name(std::move(enumName)) {}
    // Helper function
    static bool hasExplicitValue(int value) { return value != -1; }
};


// Union declaration
class UnionDecl : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> members;

    explicit UnionDecl(std::string unionName)
        : ASTNode(ASTNodeType::UNION_DECL), name(std::move(unionName)) {}
};


// Function declaration
class FunctionDecl : public ASTNode {
public:
    std::string name;
    std::unique_ptr<ASTNode> returnType; // type node
    std::vector<std::unique_ptr<ASTNode>> parameters; // VarDecl or similar
    std::unique_ptr<ASTNode> body; // BlockStmt or expression (for lambdas)
    bool isConst = false;
    bool isVirtual = false;
    bool isStatic = false;
    bool isConstructor = false;
    bool isDestructor = false;

    explicit FunctionDecl(std::string funcName)
        : ASTNode(ASTNodeType::FUNCTION_DECL), name(std::move(funcName)) {}
};


// Variable declaration
class VarDecl : public ASTNode {
public:
    std::string name;
    std::unique_ptr<ASTNode> type;  // type node
    std::unique_ptr<ASTNode> initializer; // optional initializer expression
    bool isStatic = false;
    bool isConst = false;

    VarDecl(std::string varName,
            std::unique_ptr<ASTNode> typeNode = nullptr,
            std::unique_ptr<ASTNode> init = nullptr,
            bool isStatic_ = false,
            bool isConst_ = false)
        : ASTNode(ASTNodeType::VAR_DECL),
          name(std::move(varName)),
          type(std::move(typeNode)),
          initializer(std::move(init)),
          isStatic(isStatic_),
          isConst(isConst_) {}
};

class InitializerListExpr : public Expression {
public:
    std::vector<std::unique_ptr<ASTNode>> elements;
    InitializerListExpr(std::vector<std::unique_ptr<ASTNode>> elems)
        : Expression(ASTNodeType::INITIALIZER_LIST_EXPR), elements(std::move(elems)) {}
};

// Typedef or using alias

class TypedefDecl : public ASTNode {
public:
    std::string aliasName;
    std::unique_ptr<ASTNode> aliasedType;

    TypedefDecl(std::string alias, std::unique_ptr<ASTNode> aliased)
        : ASTNode(ASTNodeType::TYPEDEF_DECL), aliasName(std::move(alias)), aliasedType(std::move(aliased)) {}
};


// Base Statement node (optional, for clarity)
class Statement : public ASTNode {
public:
    explicit Statement(ASTNodeType type) : ASTNode(type) {}
};


// Block statement: a sequence of statements
class BlockStmt : public Statement {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    BlockStmt() : Statement(ASTNodeType::BLOCK_STMT) {}
};


// Expression statement: a statement that is just an expression (e.g., function call)
class ExpressionStmt : public Statement {
public:
    std::unique_ptr<ASTNode> expression;

    explicit ExpressionStmt(std::unique_ptr<ASTNode> expr)
        : Statement(ASTNodeType::EXPRESSION_STMT), expression(std::move(expr)) {}
};


// If statement
class IfStmt : public Statement {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch; // can be nullptr or ElseStmt or another IfStmt

    IfStmt()
        : Statement(ASTNodeType::IF_STMT) {}
};


// Else statement (optional explicit node)
class ElseStmt : public Statement {
public:
    std::unique_ptr<ASTNode> elseBranch;

    explicit ElseStmt(std::unique_ptr<ASTNode> elseBr)
        : Statement(ASTNodeType::ELSE_STMT), elseBranch(std::move(elseBr)) {}
};


// While statement
class WhileStmt : public Statement {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;

    WhileStmt()
        : Statement(ASTNodeType::WHILE_STMT) {}
};


// Do-While statement
class DoWhileStmt : public Statement {
public:
    std::unique_ptr<ASTNode> body;
    std::unique_ptr<ASTNode> condition;

    DoWhileStmt()
        : Statement(ASTNodeType::DO_WHILE_STMT) {}
};


// For statement
class ForStmt : public Statement {
public:
    std::unique_ptr<ASTNode> init;       // e.g. VarDecl or ExpressionStmt, can be nullptr
    std::unique_ptr<ASTNode> condition;  // can be nullptr
    std::unique_ptr<ASTNode> increment;  // can be nullptr
    std::unique_ptr<ASTNode> body;

    ForStmt()
        : Statement(ASTNodeType::FOR_STMT) {}
};


// Return statement
class ReturnStmt : public Statement {
public:
    std::unique_ptr<ASTNode> expression; // optional

    ReturnStmt()
        : Statement(ASTNodeType::RETURN_STMT) {}
};


// Break statement
class BreakStmt : public Statement {
public:
    BreakStmt()
        : Statement(ASTNodeType::BREAK_STMT) {}
};


// Continue statement
class ContinueStmt : public Statement {
public:
    ContinueStmt()
        : Statement(ASTNodeType::CONTINUE_STMT) {}
};


// Goto statement
class GotoStmt : public Statement {
public:
    std::string label;

    explicit GotoStmt(std::string lbl)
        : Statement(ASTNodeType::GOTO_STMT), label(std::move(lbl)) {}
};


// Try statement
class TryStmt : public Statement {
public:
    std::unique_ptr<BlockStmt> tryBlock;
    std::vector<std::unique_ptr<ASTNode>> catchClauses;  // Catches

    TryStmt()
        : Statement(ASTNodeType::TRY_STMT) {}
};


// Catch statement
class CatchStmt : public Statement {
public:
    std::unique_ptr<ASTNode> exceptionType; // type of exception caught, e.g. std::exception
    std::string exceptionVar;                // catch (const std::exception& e)
    std::unique_ptr<BlockStmt> body;

    CatchStmt()
        : Statement(ASTNodeType::CATCH_STMT) {}
};


// Throw statement
class ThrowStmt : public Statement {
public:
    std::unique_ptr<ASTNode> expression;

    ThrowStmt()
        : Statement(ASTNodeType::THROW_STMT) {}
};

// Switch statement
class SwitchStmt : public Statement {
public:
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> cases; // CaseStmt and DefaultStmt

    SwitchStmt(std::unique_ptr<ASTNode> cond, std::vector<std::unique_ptr<ASTNode>> caseList)
        : Statement(ASTNodeType::SWITCH_STMT), condition(std::move(cond)), cases(std::move(caseList)) {}
};

// Case statement
class CaseStmt : public Statement {
public:
    std::unique_ptr<ASTNode> value; // case value expression
    std::vector<std::unique_ptr<ASTNode>> statements;

    CaseStmt(std::unique_ptr<ASTNode> val, std::vector<std::unique_ptr<ASTNode>> stmts)
        : Statement(ASTNodeType::CASE_STMT), value(std::move(val)), statements(std::move(stmts)) {}
};

// Default statement
class DefaultStmt : public Statement {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    explicit DefaultStmt(std::vector<std::unique_ptr<ASTNode>> stmts)
        : Statement(ASTNodeType::DEFAULT_STMT), statements(std::move(stmts)) {}
};


// Base Expression node
class Expression : public ASTNode {
public:
    explicit Expression(ASTNodeType type) : ASTNode(type) {}
};


// Literal expression
class Literal : public Expression {
public:
    std::string value; // raw literal text or parsed value

    explicit Literal(std::string val)
        : Expression(ASTNodeType::LITERAL), value(std::move(val)) {}
};


// Identifier expression
class Identifier : public Expression {
public:
    std::string name;

    explicit Identifier(std::string idName)
        : Expression(ASTNodeType::IDENTIFIER), name(std::move(idName)) {}
};


// Binary expression
class BinaryExpr : public Expression {
public:
    std::string op;  // operator string, e.g. "+", "==", etc.
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryExpr(std::string oper, std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs)
        : Expression(ASTNodeType::BINARY_EXPR), op(std::move(oper)),
          left(std::move(lhs)), right(std::move(rhs)) {}
};


// Unary expression
class UnaryExpr : public Expression {
public:
    std::string op;  // e.g. "-", "!", "&", "*"
    std::unique_ptr<ASTNode> operand;
    bool isPrefix;

    UnaryExpr(std::string oper, std::unique_ptr<ASTNode> opd, bool prefix)
        : Expression(ASTNodeType::UNARY_EXPR), op(std::move(oper)),
          operand(std::move(opd)), isPrefix(prefix) {}
};


// Ternary expression
class TernaryExpr : public Expression {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> trueExpr;
    std::unique_ptr<ASTNode> falseExpr;

    TernaryExpr(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> t, std::unique_ptr<ASTNode> f)
        : Expression(ASTNodeType::TERNARY_EXPR),
          condition(std::move(cond)), trueExpr(std::move(t)), falseExpr(std::move(f)) {}
};


// Function call expression
class FunctionCall : public Expression {
public:
    std::unique_ptr<ASTNode> callee; // function identifier or expression
    std::vector<std::unique_ptr<ASTNode>> arguments;

    explicit FunctionCall(std::unique_ptr<ASTNode> calleeNode)
        : Expression(ASTNodeType::FUNCTION_CALL), callee(std::move(calleeNode)) {}
};





// TemplateType, PointerType, ReferenceType, QualifiedType, etc. can be done similarly:
// Here's an example for TemplateType:
// Member access expression: obj.member or obj->member
class MemberAccess : public Expression {
public:
    std::unique_ptr<ASTNode> object;
    std::string memberName;
    bool isArrow;  // true for '->', false for '.'

    MemberAccess(std::unique_ptr<ASTNode> obj, std::string member, bool arrow)
        : Expression(ASTNodeType::MEMBER_ACCESS),
          object(std::move(obj)), memberName(std::move(member)), isArrow(arrow) {}
};


// Array access: arr[i]
class ArrayAccess : public Expression {
public:
    std::unique_ptr<ASTNode> arrayExpr;
    std::unique_ptr<ASTNode> indexExpr;

    ArrayAccess(std::unique_ptr<ASTNode> arr, std::unique_ptr<ASTNode> idx)
        : Expression(ASTNodeType::ARRAY_ACCESS),
          arrayExpr(std::move(arr)), indexExpr(std::move(idx)) {}
};

class ArrayType : public ASTNode {
public:
    std::unique_ptr<ASTNode> elementType;
    int size;
    ArrayType(std::unique_ptr<ASTNode> elem, int sz)
        : ASTNode(ASTNodeType::ARRAY_TYPE), elementType(std::move(elem)), size(sz) {}
};


// STL vector access or push_back etc.
class VectorAccess : public Expression {
public:
    std::unique_ptr<ASTNode> vectorExpr;
    std::string method; // e.g., "push_back", "size", "at"
    std::vector<std::unique_ptr<ASTNode>> arguments;

    VectorAccess(std::unique_ptr<ASTNode> vec, std::string m)
        : Expression(ASTNodeType::FUNCTION_CALL), vectorExpr(std::move(vec)), method(std::move(m)) {}
};


// std::cout, std::cin stream expression
class StreamExpr : public Expression {
public:
    std::vector<std::unique_ptr<ASTNode>> chain;  // e.g. cout << x << y;

    StreamExpr() : Expression(ASTNodeType::STREAM_EXPR) {}
};


// Lambda expression
// Use this version (line ~900)
class LambdaExpr : public Expression {
public:
    std::vector<std::string> captureList;
    std::vector<std::unique_ptr<ASTNode>> parameters; // like VarDecl
    std::unique_ptr<ASTNode> returnType;
    std::unique_ptr<ASTNode> body; // usually BlockStmt

    LambdaExpr(std::vector<std::string> captures,
               std::vector<std::unique_ptr<ASTNode>> params,
               std::unique_ptr<ASTNode> retType,
               std::unique_ptr<ASTNode> bodyExpr)
        : Expression(ASTNodeType::LAMBDA_EXPR),
          captureList(std::move(captures)),
          parameters(std::move(params)),
          returnType(std::move(retType)),
          body(std::move(bodyExpr)) {}
};

// Casts
class StaticCastExpr : public Expression {
public:
    std::unique_ptr<ASTNode> targetType;
    std::unique_ptr<ASTNode> expr;

    StaticCastExpr(std::unique_ptr<ASTNode> type, std::unique_ptr<ASTNode> e)
        : Expression(ASTNodeType::STATIC_CAST_EXPR), targetType(std::move(type)), expr(std::move(e)) {}
};


class DynamicCastExpr : public Expression {
public:
    std::unique_ptr<ASTNode> targetType;
    std::unique_ptr<ASTNode> expr;

    DynamicCastExpr(std::unique_ptr<ASTNode> type, std::unique_ptr<ASTNode> e)
        : Expression(ASTNodeType::DYNAMIC_CAST_EXPR), targetType(std::move(type)), expr(std::move(e)) {}
};


class ConstCastExpr : public Expression {
public:
    std::unique_ptr<ASTNode> targetType;
    std::unique_ptr<ASTNode> expr;

    ConstCastExpr(std::unique_ptr<ASTNode> type, std::unique_ptr<ASTNode> e)
        : Expression(ASTNodeType::CONST_CAST_EXPR), targetType(std::move(type)), expr(std::move(e)) {}
};


class ReinterpretCastExpr : public Expression {
public:
    std::unique_ptr<ASTNode> targetType;
    std::unique_ptr<ASTNode> expr;

    ReinterpretCastExpr(std::unique_ptr<ASTNode> type, std::unique_ptr<ASTNode> e)
        : Expression(ASTNodeType::REINTERPRET_CAST_EXPR), targetType(std::move(type)), expr(std::move(e)) {}
};


// typeid(expr)
class TypeidExpr : public Expression {
public:
    std::unique_ptr<ASTNode> expr;

    explicit TypeidExpr(std::unique_ptr<ASTNode> e)
        : Expression(ASTNodeType::TYPEID_EXPR), expr(std::move(e)) {}
};


// Template type node (like vector<T>, map<K,V>)
class TemplateClassDecl : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<TemplateParam>> templateParams;
    std::vector<std::unique_ptr<ASTNode>> members;

    TemplateClassDecl(
        const std::string& name,
        std::vector<std::unique_ptr<TemplateParam>> params,
        std::vector<std::unique_ptr<ASTNode>> members)
        : ASTNode(ASTNodeType::TEMPLATE_CLASS_DECL),
          name(name),
          templateParams(std::move(params)),
          members(std::move(members)) {}
};


class TemplateType : public ASTNode {
public:
    std::string baseTypeName;  // e.g. "vector", "map"
    std::vector<std::unique_ptr<ASTNode>> typeArgs;

    explicit TemplateType(std::string base)
        : ASTNode(ASTNodeType::TEMPLATE_TYPE), baseTypeName(std::move(base)) {}
};


// Template parameter (T, typename U, etc.)
class TemplateParam : public ASTNode {
public:
    std::string name;

    explicit TemplateParam(std::string paramName)
        : ASTNode(ASTNodeType::TEMPLATE_PARAM), name(std::move(paramName)) {}
};


// Template argument (type or expression)
class TemplateArg : public ASTNode {
public:
    std::unique_ptr<ASTNode> arg;

    explicit TemplateArg(std::unique_ptr<ASTNode> a)
        : ASTNode(ASTNodeType::TEMPLATE_ARG), arg(std::move(a)) {}
};

class TemplateFunctionDecl : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<TemplateParam>> templateParams;
    std::unique_ptr<ASTNode> returnType;
    std::vector<std::unique_ptr<VarDecl>> parameters;
    std::unique_ptr<ASTNode> body;

    TemplateFunctionDecl(
        std::string n,
        std::vector<std::unique_ptr<TemplateParam>> tparams,
        std::unique_ptr<ASTNode> retType,
        std::vector<std::unique_ptr<VarDecl>> params,
        std::unique_ptr<ASTNode> b
    )
        : ASTNode(ASTNodeType::TEMPLATE_FUNCTION_DECL),
          name(std::move(n)),
          templateParams(std::move(tparams)),
          returnType(std::move(retType)),
          parameters(std::move(params)),
          body(std::move(b)) {}
};

// Qualified type (e.g., const int&, std::string, long long)
class QualifiedType : public ASTNode {
public:
    std::string name;  // e.g., "std::vector"
    bool isConst = false;
    bool isPointer = false;
    bool isReference = false;

    explicit QualifiedType(std::string tname)
        : ASTNode(ASTNodeType::QUALIFIED_TYPE), name(std::move(tname)) {}
};

class QualifiedName : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string right;

    QualifiedName(std::unique_ptr<ASTNode> left, std::string right)
        : ASTNode(ASTNodeType::QUALIFIED_TYPE), left(std::move(left)), right(std::move(right)) {}
};

// Pointer type
class PointerType : public ASTNode {
public:
    std::unique_ptr<ASTNode> baseType;

    explicit PointerType(std::unique_ptr<ASTNode> base)
        : ASTNode(ASTNodeType::POINTER_TYPE), baseType(std::move(base)) {}
};


// Reference type
class ReferenceType : public ASTNode {
public:
    std::unique_ptr<ASTNode> baseType;

    explicit ReferenceType(std::unique_ptr<ASTNode> base)
        : ASTNode(ASTNodeType::REFERENCE_TYPE), baseType(std::move(base)) {}
};


// ---- Concurrency & Threads ---- //

// std::thread
class ThreadDecl : public ASTNode {
public:
    std::string threadVarName;
    std::unique_ptr<ASTNode> callable;

    ThreadDecl(std::string name, std::unique_ptr<ASTNode> call)
        : ASTNode(ASTNodeType::THREAD_DECL), threadVarName(std::move(name)), callable(std::move(call)) {}
};


// std::mutex
class MutexDecl : public ASTNode {
public:
    std::string name;

    explicit MutexDecl(std::string n)
        : ASTNode(ASTNodeType::MUTEX_DECL), name(std::move(n)) {}
};


// std::lock_guard or std::unique_lock
class LockGuardDecl : public ASTNode {
public:
    std::string guardType; // "lock_guard" or "unique_lock"
    std::string varName;
    std::string mutexName;

    LockGuardDecl(std::string type, std::string var, std::string mutex)
        : ASTNode(ASTNodeType::LOCK_GUARD_DECL),
          guardType(std::move(type)), varName(std::move(var)), mutexName(std::move(mutex)) {}
};


// std::async
class AsyncExpr : public Expression {
public:
    std::unique_ptr<ASTNode> callable;
    std::vector<std::unique_ptr<ASTNode>> arguments;

    AsyncExpr() : Expression(ASTNodeType::ASYNC_EXPR) {}
};


// std::future
class FutureExpr : public Expression {
public:
    std::string futureName;

    explicit FutureExpr(std::string name)
        : Expression(ASTNodeType::FUTURE_EXPR), futureName(std::move(name)) {}
};


// std::promise
class PromiseDecl : public ASTNode {
public:
    std::string name;

    explicit PromiseDecl(std::string name)
        : ASTNode(ASTNodeType::PROMISE_DECL), name(std::move(name)) {}
};

class MathFunctionCall : public Expression {
public:
    std::string functionName; // e.g., "abs", "sqrt", "pow"
    std::vector<std::unique_ptr<ASTNode>> arguments;

    explicit MathFunctionCall(std::string name,ASTNodeType type)
        : Expression(type), functionName(std::move(name)) {}
};



// Exception classes (e.g., std::runtime_error)
class RuntimeErrorClass : public ASTNode {
public:
    std::string message;

    explicit RuntimeErrorClass(std::string msg)
        : ASTNode(ASTNodeType::RUNTIME_ERROR_CLASS), message(std::move(msg)) {}
};

// STL container example: std::vector<T> variable
class VectorTypeExpr : public Expression {
public:
    std::vector<std::unique_ptr<ASTNode>> typeParams;

    explicit VectorTypeExpr(std::vector<std::unique_ptr<ASTNode>> params)
        : Expression(ASTNodeType::VECTOR_TYPE), typeParams(std::move(params)) {}
};

// Algorithms
class SortCall : public Expression {
public:
    std::unique_ptr<ASTNode> container;

    explicit SortCall(std::unique_ptr<ASTNode> cont)
        : Expression(ASTNodeType::SORT_CALL), container(std::move(cont)) {}
};

class AccumulateCall : public Expression {
public:
    std::unique_ptr<ASTNode> beginExpr;
    std::unique_ptr<ASTNode> endExpr;
    std::unique_ptr<ASTNode> initialValue;

    AccumulateCall(std::unique_ptr<ASTNode> b, std::unique_ptr<ASTNode> e, std::unique_ptr<ASTNode> init)
        : Expression(ASTNodeType::ACCUMULATE_CALL), beginExpr(std::move(b)), endExpr(std::move(e)), initialValue(std::move(init)) {}
};

class FindCall : public Expression {
public:
    std::unique_ptr<ASTNode> container;
    std::unique_ptr<ASTNode> value;
    FindCall(std::unique_ptr<ASTNode> cont, std::unique_ptr<ASTNode> val)
        : Expression(ASTNodeType::FIND_CALL), container(std::move(cont)), value(std::move(val)) {}
};

// I/O stream nodes
class CoutExpr : public Expression {
public:
    std::vector<std::unique_ptr<ASTNode>> outputValues;

    explicit CoutExpr(std::vector<std::unique_ptr<ASTNode>> values)
        : Expression(ASTNodeType::COUT_EXPR), outputValues(std::move(values)) {}
};

class CinExpr : public Expression {
public:
    std::vector<std::unique_ptr<ASTNode>> inputTargets;

    explicit CinExpr(std::vector<std::unique_ptr<ASTNode>> targets)
        : Expression(ASTNodeType::CIN_EXPR), inputTargets(std::move(targets)) {}
};

class CerrExpr : public Expression {
public:
    std::vector<std::unique_ptr<ASTNode>> errorOutputs;

    explicit CerrExpr(std::vector<std::unique_ptr<ASTNode>> errors)
        : Expression(ASTNodeType::CERR_EXPR), errorOutputs(std::move(errors)) {}
};

class GetlineCall : public Expression {
public:
    std::unique_ptr<ASTNode> streamExpr; // e.g., std::cin
    std::unique_ptr<ASTNode> targetVar;  // variable to store the line

    GetlineCall(std::unique_ptr<ASTNode> stream, std::unique_ptr<ASTNode> target)
        : Expression(ASTNodeType::GETLINE_CALL), streamExpr(std::move(stream)), targetVar(std::move(target)) {}
};

class PrintfCall : public Expression {
public:
    std::vector<std::unique_ptr<ASTNode>> arguments;
    PrintfCall(std::vector<std::unique_ptr<ASTNode>> args)
        : Expression(ASTNodeType::PRINTF_CALL), arguments(std::move(args)) {}
};

class ScanfCall : public Expression {
public:
    std::vector<std::unique_ptr<ASTNode>> inputTargets;
    ScanfCall(std::vector<std::unique_ptr<ASTNode>> targets)
        : Expression(ASTNodeType::SCANF_CALL), inputTargets(std::move(targets)) {}
};

class NewExpr : public Expression {
public:
    std::unique_ptr<ASTNode> type;
    std::vector<std::unique_ptr<ASTNode>> args;

    NewExpr(std::unique_ptr<ASTNode> type, std::vector<std::unique_ptr<ASTNode>> args)
        : Expression(ASTNodeType::NEW_EXPR), type(std::move(type)), args(std::move(args)) {}
};

class DeleteExpr : public Expression {
public:
    std::unique_ptr<ASTNode> expr;

    explicit DeleteExpr(std::unique_ptr<ASTNode> expr)
        : Expression(ASTNodeType::DELETE_EXPR), expr(std::move(expr)) {}
};

class MallocCall : public Expression {
public:
    std::unique_ptr<ASTNode> sizeExpr;
    std::unique_ptr<ASTNode> elementType; // Add this line

    MallocCall(std::unique_ptr<ASTNode> size, std::unique_ptr<ASTNode> elemType)
        : Expression(ASTNodeType::MALLOC_CALL), sizeExpr(std::move(size)), elementType(std::move(elemType)) {}
};

class FreeCall : public Expression {
public:
    std::unique_ptr<ASTNode> ptrExpr;
    FreeCall(std::unique_ptr<ASTNode> ptr)
        : Expression(ASTNodeType::FREE_CALL), ptrExpr(std::move(ptr)) {}
};

class AbsCall : public Expression {
public:
    std::unique_ptr<ASTNode> valueExpr;
    AbsCall(std::unique_ptr<ASTNode> val)
        : Expression(ASTNodeType::ABS_CALL), valueExpr(std::move(val)) {}
};


class PreprocessorInclude : public ASTNode {
public:
    std::string header;
    explicit PreprocessorInclude(std::string header)
        : ASTNode(ASTNodeType::PREPROCESSOR_INCLUDE), header(std::move(header)) {}
};

class PreprocessorDefine : public ASTNode {
public:
    std::string macro;
    std::string value;
    PreprocessorDefine(std::string macro, std::string value)
        : ASTNode(ASTNodeType::PREPROCESSOR_DEFINE), macro(std::move(macro)), value(std::move(value)) {}
};

class PreprocessorUndef : public ASTNode {
public:
    std::string macro;
    explicit PreprocessorUndef(std::string macro)
        : ASTNode(ASTNodeType::PREPROCESSOR_UNDEF), macro(std::move(macro)) {}
};

class PreprocessorIfdef : public ASTNode {
public:
    std::string macro;
    explicit PreprocessorIfdef(std::string macro)
        : ASTNode(ASTNodeType::PREPROCESSOR_IFDEF), macro(std::move(macro)) {}
};

class PreprocessorIfndef : public ASTNode {
public:
    std::string macro;
    explicit PreprocessorIfndef(std::string macro)
        : ASTNode(ASTNodeType::PREPROCESSOR_IFNDEF), macro(std::move(macro)) {}
};

class PreprocessorIf : public ASTNode {
public:
    std::string condition;
    explicit PreprocessorIf(std::string condition)
        : ASTNode(ASTNodeType::PREPROCESSOR_IF), condition(std::move(condition)) {}
};

class PreprocessorElse : public ASTNode {
public:
    PreprocessorElse()
        : ASTNode(ASTNodeType::PREPROCESSOR_ELSE) {}
};

class PreprocessorElif : public ASTNode {
public:
    std::string condition;
    explicit PreprocessorElif(std::string condition)
        : ASTNode(ASTNodeType::PREPROCESSOR_ELIF), condition(std::move(condition)) {}
};

class PreprocessorEndif : public ASTNode {
public:
    PreprocessorEndif()
        : ASTNode(ASTNodeType::PREPROCESSOR_ENDIF) {}
};

class PreprocessorPragma : public ASTNode {
public:
    std::string pragma;
    explicit PreprocessorPragma(std::string pragma)
        : ASTNode(ASTNodeType::PREPROCESSOR_PRAGMA), pragma(std::move(pragma)) {}
};

class PreprocessorUnknown : public ASTNode {
public:
    std::string text;
    explicit PreprocessorUnknown(std::string text)
        : ASTNode(ASTNodeType::PREPROCESSOR_UNKNOWN), text(std::move(text)) {}
};
// You can follow the same pattern for the remaining STL containers (map, set, list, etc.) and other standard function calls.
#endif