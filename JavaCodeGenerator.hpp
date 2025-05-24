#ifndef JAVA_CODE_GENERATOR_HPP
#define JAVA_CODE_GENERATOR_HPP

#include <string>
#include <set>
#include "ast.hpp"

class JavaCodeGenerator {
public:
    std::string generate(const ASTNode* node, const std::string& className = "Main") const;
    mutable std::unordered_map<std::string, const ASTNode*> symbolTable;
    mutable std::set<std::string> requiredImports;
    std::set<std::string> userDefinedTemplates;


private:
    // Main generators for top-level constructs
    std::string generateFunctionDecl(const FunctionDecl* node, const std::string& className) const;
    std::string generateVarDecl(const VarDecl* node) const;
    std::string generateBlockStmt(const BlockStmt* node, const std::string& className) const;
    std::string generateIfStmt(const IfStmt* node, const std::string& className) const;
    std::string generateReturnStmt(const ReturnStmt* node, const std::string& className) const;

    // Expressions
    std::string generateBinaryExpr(const BinaryExpr* node, const std::string& className) const;
    std::string generateLiteral(const Literal* node) const;
    std::string generateIdentifier(const Identifier* node) const;

    // Type mapping
    std::string mapTypeNodeToJava(const ASTNode* typeNode, bool forGeneric = false) const;
    std::string mapCppTypeNameToJava(const std::string& cppType, bool forGeneric = false) const;    

    std::string generateClassDecl(const ClassDecl* node, const std::string& className) const;
    std::string generateStructDecl(const StructDecl* node, const std::string& className) const;
    std::string generateEnumDecl(const EnumDecl* node) const;

    std::string generateForStmt(const ForStmt* node, const std::string& className) const;
    std::string generateWhileStmt(const WhileStmt* node, const std::string& className) const;
    std::string generateDoWhileStmt(const DoWhileStmt* node, const std::string& className) const;
    std::string generateBreakStmt(const BreakStmt* node, const std::string& className) const;
    std::string generateContinueStmt(const ContinueStmt* node, const std::string& className) const;
    std::string generateExpressionStmt(const ExpressionStmt* node, const std::string& className) const;

    std::string generateUnaryExpr(const UnaryExpr* node, const std::string& className) const;
    std::string generateTernaryExpr(const TernaryExpr* node, const std::string& className) const;
    std::string generateFunctionCall(const FunctionCall* node, const std::string& className) const;
    std::string generateMemberAccess(const MemberAccess* node, const std::string& className) const;
    std::string generateArrayAccess(const ArrayAccess* node, const std::string& className) const;
    std::string generateSwitchStmt(const SwitchStmt* node, const std::string& className) const;
    std::string generateCaseStmt(const CaseStmt* node, const std::string& className) const;
    std::string generateDefaultStmt(const DefaultStmt* node, const std::string& className) const;
    std::string generateSortCall(const SortCall* node, const std::string& className) const;
    std::string generateFindCall(const FindCall* node, const std::string& className) const;
    std::string generateAccumulateCall(const AccumulateCall* node, const std::string& className) const;
    std::string generateVectorAccess(const VectorAccess* node, const std::string& className) const;
    std::string generateCoutExpr(const CoutExpr* node, const std::string& className) const;
    std::string generateCerrExpr(const CerrExpr* node, const std::string& className) const;
    std::string generateCinExpr(const CinExpr* node, const std::string& className) const;
    std::string generateGetlineCall(const GetlineCall* node, const std::string& className) const;
    std::string generatePrintfCall(const PrintfCall* node, const std::string& className) const;
    std::string generateScanfCall(const ScanfCall* node, const std::string& className) const;
    std::string generateMallocCall(const MallocCall* node, const std::string& className) const;
    std::string generateFreeCall(const FreeCall* node, const std::string& className) const;
    std::string generateAbsCall(const AbsCall* node, const std::string& className) const;
    std::string generateTemplateClassDecl(const TemplateClassDecl* node, const std::string& className) const;
    std::string generateTemplateFunctionDecl(const TemplateFunctionDecl* node, const std::string& className) const;
    std::string generateInitializerListExpr(const InitializerListExpr* node) const;
};

#endif