#include "JavaCodeGenerator.hpp"
#include <sstream>
#include <unordered_map>
#include <stdexcept>

// --- Main dispatcher ---
std::string JavaCodeGenerator::generate(const ASTNode* node, const std::string& className) const {
    if (!node) return "";
    switch (node->type) {
    case ASTNodeType::FUNCTION_DECL:
        return generateFunctionDecl(static_cast<const FunctionDecl*>(node), className);
    case ASTNodeType::VAR_DECL:
        return generateVarDecl(static_cast<const VarDecl*>(node));
    case ASTNodeType::BLOCK_STMT:
        return generateBlockStmt(static_cast<const BlockStmt*>(node), className);
    case ASTNodeType::IF_STMT:
        return generateIfStmt(static_cast<const IfStmt*>(node), className);
    case ASTNodeType::RETURN_STMT:
        return generateReturnStmt(static_cast<const ReturnStmt*>(node), className);
    case ASTNodeType::BINARY_EXPR:
        return generateBinaryExpr(static_cast<const BinaryExpr*>(node), className);
    case ASTNodeType::LITERAL:
        return generateLiteral(static_cast<const Literal*>(node));
    case ASTNodeType::IDENTIFIER:
        return generateIdentifier(static_cast<const Identifier*>(node));
    case ASTNodeType::CLASS_DECL:
        return generateClassDecl(static_cast<const ClassDecl*>(node), className);
    case ASTNodeType::STRUCT_DECL:
        return generateStructDecl(static_cast<const StructDecl*>(node), className);
    case ASTNodeType::ENUM_DECL:
        return generateEnumDecl(static_cast<const EnumDecl*>(node));
    case ASTNodeType::FOR_STMT:
        return generateForStmt(static_cast<const ForStmt*>(node), className);
    case ASTNodeType::WHILE_STMT:
        return generateWhileStmt(static_cast<const WhileStmt*>(node), className);
    case ASTNodeType::DO_WHILE_STMT:
        return generateDoWhileStmt(static_cast<const DoWhileStmt*>(node), className);
    case ASTNodeType::BREAK_STMT:
        return generateBreakStmt(static_cast<const BreakStmt*>(node), className);
    case ASTNodeType::CONTINUE_STMT:
        return generateContinueStmt(static_cast<const ContinueStmt*>(node), className);
    case ASTNodeType::EXPRESSION_STMT:
        return generateExpressionStmt(static_cast<const ExpressionStmt*>(node), className);
    case ASTNodeType::UNARY_EXPR:
        return generateUnaryExpr(static_cast<const UnaryExpr*>(node), className);
    case ASTNodeType::TERNARY_EXPR:
        return generateTernaryExpr(static_cast<const TernaryExpr*>(node), className);
    case ASTNodeType::FUNCTION_CALL:
        return generateFunctionCall(static_cast<const FunctionCall*>(node), className);
    case ASTNodeType::MEMBER_ACCESS:
        return generateMemberAccess(static_cast<const MemberAccess*>(node), className);
    case ASTNodeType::ARRAY_ACCESS:
        return generateArrayAccess(static_cast<const ArrayAccess*>(node), className);
    case ASTNodeType::SWITCH_STMT:
        return generateSwitchStmt(static_cast<const SwitchStmt*>(node), className);
    case ASTNodeType::CASE_STMT:
        return generateCaseStmt(static_cast<const CaseStmt*>(node), className);
    case ASTNodeType::DEFAULT_STMT:
        return generateDefaultStmt(static_cast<const DefaultStmt*>(node), className);    
    case ASTNodeType::SORT_CALL:
        return generateSortCall(static_cast<const SortCall*>(node), className);
    case ASTNodeType::FIND_CALL:
        return generateFindCall(static_cast<const FindCall*>(node), className);
    case ASTNodeType::ACCUMULATE_CALL:
        return generateAccumulateCall(static_cast<const AccumulateCall*>(node), className);
    case ASTNodeType::COUT_EXPR:
        return generateCoutExpr(static_cast<const CoutExpr*>(node), className);
    case ASTNodeType::CERR_EXPR:
        return generateCerrExpr(static_cast<const CerrExpr*>(node), className);
    case ASTNodeType::CIN_EXPR:
        return generateCinExpr(static_cast<const CinExpr*>(node), className);
    case ASTNodeType::GETLINE_CALL:
        return generateGetlineCall(static_cast<const GetlineCall*>(node), className);   
    case ASTNodeType::PRINTF_CALL:
        return generatePrintfCall(static_cast<const PrintfCall*>(node), className);
    case ASTNodeType::SCANF_CALL:
        return generateScanfCall(static_cast<const ScanfCall*>(node), className);
    case ASTNodeType::MALLOC_CALL:
    return generateMallocCall(static_cast<const MallocCall*>(node), className);
    case ASTNodeType::FREE_CALL:
        return generateFreeCall(static_cast<const FreeCall*>(node), className);
    case ASTNodeType::ABS_CALL:
        return generateAbsCall(static_cast<const AbsCall*>(node), className);
    case ASTNodeType::TEMPLATE_CLASS_DECL:
        return generateTemplateClassDecl(static_cast<const TemplateClassDecl*>(node), className);    
    case ASTNodeType::TEMPLATE_FUNCTION_DECL:
        return generateTemplateFunctionDecl(static_cast<const TemplateFunctionDecl*>(node), className);
    case ASTNodeType::INITIALIZER_LIST_EXPR:
    return generateInitializerListExpr(static_cast<const InitializerListExpr*>(node));
    default:
        return "// Unsupported AST node\n";
}
}

// --- Function Declaration ---
std::string JavaCodeGenerator::generateFunctionDecl(const FunctionDecl* node, const std::string& className) const {
    std::ostringstream oss;
    // Java: function must be inside a class
    oss << "public class " << className << " {\n";
    oss << "    public static " << mapTypeNodeToJava(node->returnType.get()) << " " << node->name << "(";
    // Parameters
    for (size_t i = 0; i < node->parameters.size(); ++i) {
        const VarDecl* param = static_cast<const VarDecl*>(node->parameters[i].get());
        oss << mapTypeNodeToJava(param->type.get()) << " " << param->name;
        if (i + 1 < node->parameters.size()) oss << ", ";
    }
    oss << ")";
    // Function body
    if (node->body) {
        oss << " " << generate(node->body.get(), className);
    } else {
        oss << " {}";
    }
    oss << "\n}\n";
    return oss.str();
}

// --- Variable Declaration ---
std::string JavaCodeGenerator::generateVarDecl(const VarDecl* node) const {
    std::ostringstream oss;
    std::string typeStr = mapTypeNodeToJava(node->type.get());
    oss << typeStr << " " << node->name;
    symbolTable[node->name] = node->type.get();

    // --- User-defined template class instantiation ---
    if (node->type && node->type->type == ASTNodeType::TEMPLATE_TYPE) {
        const TemplateType* tt = static_cast<const TemplateType*>(node->type.get());
        if (userDefinedTemplates.count(tt->baseTypeName)) {
            oss << " = new " << tt->baseTypeName << "<";
            for (size_t i = 0; i < tt->typeArgs.size(); ++i) {
                oss << mapTypeNodeToJava(tt->typeArgs[i].get(), true);
                if (i + 1 < tt->typeArgs.size()) oss << ", ";
            }
            oss << ">()";
        }
    }
    // --- Existing STL container/initializer logic ---
    else {
        // --- Add imports for generic interfaces ---
        if (typeStr.find("List") == 0)        requiredImports.insert("import java.util.List;");
        if (typeStr.find("Map") == 0)         requiredImports.insert("import java.util.Map;");
        if (typeStr.find("Set") == 0)         requiredImports.insert("import java.util.Set;");

        // --- Add imports for concrete container types ---
        if (typeStr.find("ArrayList") == 0)       requiredImports.insert("import java.util.ArrayList;");
        if (typeStr.find("HashMap") == 0)         requiredImports.insert("import java.util.HashMap;");
        if (typeStr.find("HashSet") == 0)         requiredImports.insert("import java.util.HashSet;");
        if (typeStr.find("LinkedList") == 0)      requiredImports.insert("import java.util.LinkedList;");
        if (typeStr.find("ArrayDeque") == 0)      requiredImports.insert("import java.util.ArrayDeque;");
        if (typeStr.find("Stack") == 0)           requiredImports.insert("import java.util.Stack;");
        if (typeStr.find("PriorityQueue") == 0)   requiredImports.insert("import java.util.PriorityQueue;");
        if (typeStr.find("BitSet") == 0)          requiredImports.insert("import java.util.BitSet;");
        if (typeStr.find("Optional") == 0)        requiredImports.insert("import java.util.Optional;");
        if (typeStr.find("AbstractMap") == 0)     requiredImports.insert("import java.util.AbstractMap;");

        // --- For Queue, import both interface and LinkedList if used as implementation ---
        if (typeStr.find("Queue") == 0)           requiredImports.insert("import java.util.Queue;");
        if (typeStr.find("Queue") == 0 || typeStr.find("LinkedList") == 0)
            requiredImports.insert("import java.util.LinkedList;");

        // --- Handle initializer list for containers ---
        static const std::vector<std::pair<std::string, std::string>> containerTypes = {
            {"ArrayList", "ArrayList"}, {"HashMap", "HashMap"}, {"HashSet", "HashSet"},
            {"LinkedList", "LinkedList"}, {"ArrayDeque", "ArrayDeque"}, {"Stack", "Stack"},
            {"PriorityQueue", "PriorityQueue"}, {"BitSet", "BitSet"}, {"Optional", "Optional"},
            {"Queue", "LinkedList"}
        };

        std::string instType;
        for (const auto& pair : containerTypes) {
            if (typeStr.find(pair.first) == 0) {
                instType = pair.second;
                break;
            }
        }

        if (node->initializer && node->initializer->type == ASTNodeType::INITIALIZER_LIST_EXPR && !instType.empty()) {
            requiredImports.insert("import java.util.Arrays;");
            const InitializerListExpr* initList = static_cast<const InitializerListExpr*>(node->initializer.get());
            oss << " = new " << instType << "<>(Arrays.asList(";
            for (size_t i = 0; i < initList->elements.size(); ++i) {
                oss << generate(initList->elements[i].get());
                if (i + 1 < initList->elements.size()) oss << ", ";
            }
            oss << "))";
        } else if (!node->initializer && !instType.empty()) {
            oss << " = new " << instType << "<>()";
        } else if (node->initializer) {
            oss << " = " << generate(node->initializer.get());
        }
    }

    oss << ";";
    return oss.str();
}

// --- Block Statement ---
std::string JavaCodeGenerator::generateBlockStmt(const BlockStmt* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "{\n";
    for (const auto& stmt : node->statements) {
        oss << "    " << generate(stmt.get(), className) << "\n";
    }
    oss << "}";
    return oss.str();
}

// --- If Statement ---
std::string JavaCodeGenerator::generateIfStmt(const IfStmt* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "if (" << generate(node->condition.get(), className) << ") ";
    if (node->thenBranch) {
        oss << generate(node->thenBranch.get(), className);
    }
    if (node->elseBranch) {
        oss << " else " << generate(node->elseBranch.get(), className);
    }
    return oss.str();
}

// --- Return Statement ---
std::string JavaCodeGenerator::generateReturnStmt(const ReturnStmt* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "return";
    if (node->expression) {
        oss << " " << generate(node->expression.get(), className);
    }
    oss << ";";
    return oss.str();
}

// --- Binary Expression ---
std::string JavaCodeGenerator::generateBinaryExpr(const BinaryExpr* node, const std::string& className) const {
    std::ostringstream oss;
    std::string left = generate(node->left.get(), className);
    std::string right = generate(node->right.get(), className);

    // Handle nullptr/null mapping
    if (right == "nullptr") right = "null";
    if (left == "nullptr") left = "null";

    oss << "(" << left << " " << node->op << " " << right << ")";
    return oss.str();
}

// --- Literal ---
std::string JavaCodeGenerator::generateLiteral(const Literal* node) const {
    return node->value;
}

// --- Identifier ---
std::string JavaCodeGenerator::generateIdentifier(const Identifier* node) const {
    return node->name;
}

// --- Type Mapping: ASTNode* to Java type string ---

std::string JavaCodeGenerator::mapTypeNodeToJava(const ASTNode* typeNode, bool forGeneric) const {
    if (!typeNode) return "void";
    switch (typeNode->type) {
        case ASTNodeType::QUALIFIED_TYPE: {
            const QualifiedType* qt = static_cast<const QualifiedType*>(typeNode);
            return mapCppTypeNameToJava(qt->name, forGeneric);
        }
        case ASTNodeType::TEMPLATE_TYPE: {
            const TemplateType* tt = static_cast<const TemplateType*>(typeNode);
            std::ostringstream oss;
            oss << mapCppTypeNameToJava(tt->baseTypeName, forGeneric) << "<";
            for (size_t i = 0; i < tt->typeArgs.size(); ++i) {
                oss << mapTypeNodeToJava(tt->typeArgs[i].get(), true); // true for generic context
                if (i + 1 < tt->typeArgs.size()) oss << ", ";
            }
            oss << ">";
            return oss.str();
        }
        case ASTNodeType::POINTER_TYPE:
        case ASTNodeType::REFERENCE_TYPE: {
            const auto* ptr = static_cast<const PointerType*>(typeNode);
            return mapTypeNodeToJava(ptr->baseType.get(), forGeneric);
        }
        default:
            return "Object";
    }
}

// --- Type Mapping: C++ type name to Java type name ---
std::string JavaCodeGenerator::mapCppTypeNameToJava(const std::string& cppType, bool forGeneric) const {
    static const std::unordered_map<std::string, std::string> primitiveMap = {
        {"int", "int"}, {"float", "float"}, {"double", "double"}, {"char", "char"}, {"bool", "boolean"}
    };
    static const std::unordered_map<std::string, std::string> wrapperMap = {
        {"int", "Integer"}, {"float", "Float"}, {"double", "Double"}, {"char", "Character"}, {"bool", "Boolean"}
    };
    static const std::unordered_map<std::string, std::string> typeMap = {
        {"std::string", "String"}, {"string", "String"}, {"void", "void"},
        {"vector", "ArrayList"}, {"std::vector", "ArrayList"},
        {"deque", "ArrayDeque"}, {"std::deque", "ArrayDeque"},
        {"list", "LinkedList"}, {"std::list", "LinkedList"},
        {"map", "HashMap"}, {"std::map", "HashMap"},
        {"unordered_map", "HashMap"}, {"std::unordered_map", "HashMap"},
        {"set", "HashSet"}, {"std::set", "HashSet"},
        {"unordered_set", "HashSet"}, {"std::unordered_set", "HashSet"},
        {"multimap", "HashMap"}, {"std::multimap", "HashMap"},
        {"multiset", "HashSet"}, {"std::multiset", "HashSet"},
        {"stack", "Stack"}, {"std::stack", "Stack"},
        {"queue", "Queue"}, {"std::queue", "Queue"},
        {"priority_queue", "PriorityQueue"}, {"std::priority_queue", "PriorityQueue"},
        {"bitset", "BitSet"}, {"std::bitset", "BitSet"},
        {"array", "ArrayList"}, {"std::array", "ArrayList"},
        {"pair", "AbstractMap.SimpleEntry"}, {"std::pair", "AbstractMap.SimpleEntry"},
        {"tuple", "Object[]"}, {"std::tuple", "Object[]"},
        {"optional", "Optional"}, {"std::optional", "Optional"},
        {"variant", "Object"}, {"std::variant", "Object"},
        {"any", "Object"}, {"std::any", "Object"},
    };

    if (forGeneric) {
        auto it = wrapperMap.find(cppType);
        if (it != wrapperMap.end()) return it->second;
    } else {
        auto it = primitiveMap.find(cppType);
        if (it != primitiveMap.end()) return it->second;
    }
    auto it = typeMap.find(cppType);
    if (it != typeMap.end()) return it->second;
    return cppType;
}

// --- Class/Struct/Enum Translation ---
std::string JavaCodeGenerator::generateClassDecl(const ClassDecl* node, const std::string& /*className*/) const {
    std::ostringstream oss;
    oss << "public class " << node->name << " {\n";
    // Fields
    for (const auto& member : node->members) {
        if (member->type == ASTNodeType::VAR_DECL) {
            oss << "    " << generateVarDecl(static_cast<const VarDecl*>(member.get())) << "\n";
        }
    }
    // Methods
    for (const auto& member : node->members) {
        if (member->type == ASTNodeType::FUNCTION_DECL) {
            oss << "    " << generateFunctionDecl(static_cast<const FunctionDecl*>(member.get()), node->name) << "\n";
        }
    }
    oss << "}\n";
    return oss.str();
}

std::string JavaCodeGenerator::generateStructDecl(const StructDecl* node, const std::string& /*className*/) const {
    // In Java, struct is just a class
    return generateClassDecl(reinterpret_cast<const ClassDecl*>(node), node->name);
}

std::string JavaCodeGenerator::generateEnumDecl(const EnumDecl* node) const {
    std::ostringstream oss;
    oss << "public enum " << node->name << " { ";
    for (size_t i = 0; i < node->enumerators.size(); ++i) {
        // If enumerator is a pair<string, int> or similar:
        oss << node->enumerators[i].first;
        if (i + 1 < node->enumerators.size()) oss << ", ";
    }
    oss << " }\n";
    return oss.str();
}

// --- Update dispatcher ---
std::string JavaCodeGenerator::generate(const ASTNode* node, const std::string& className) const {
    if (!node) return "";
    switch (node->type) {
        case ASTNodeType::CLASS_DECL:
            return generateClassDecl(static_cast<const ClassDecl*>(node), className);
        case ASTNodeType::STRUCT_DECL:
            return generateStructDecl(static_cast<const StructDecl*>(node), className);
        case ASTNodeType::ENUM_DECL:
            return generateEnumDecl(static_cast<const EnumDecl*>(node));
        // ...existing cases...
        default:
            return "// Unsupported AST node\n";
    }
}

std::string JavaCodeGenerator::generateForStmt(const ForStmt* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "for (";
    if (node->init) oss << generate(node->init.get(), className);
    oss << "; ";
    if (node->condition) oss << generate(node->condition.get(), className);
    oss << "; ";
    if (node->increment) oss << generate(node->increment.get(), className);
    oss << ") ";
    if (node->body) oss << generate(node->body.get(), className);
    return oss.str();
}


std::string JavaCodeGenerator::generateWhileStmt(const WhileStmt* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "while (" << generate(node->condition.get(), className) << ") ";
    if (node->body) oss << generate(node->body.get(), className);
    return oss.str();
}

std::string JavaCodeGenerator::generateDoWhileStmt(const DoWhileStmt* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "do ";
    if (node->body) oss << generate(node->body.get(), className);
    oss << " while (" << generate(node->condition.get(), className) << ");";
    return oss.str();
}

std::string JavaCodeGenerator::generateBreakStmt(const BreakStmt*, const std::string&) const {
    return "break;";
}
std::string JavaCodeGenerator::generateContinueStmt(const ContinueStmt*, const std::string&) const {
    return "continue;";
}

std::string JavaCodeGenerator::generateExpressionStmt(const ExpressionStmt* node, const std::string& className) const {
    return generate(node->expression.get(), className) + ";";
}

std::string JavaCodeGenerator::generateUnaryExpr(const UnaryExpr* node, const std::string& className) const {
    std::ostringstream oss;
    if (node->isPrefix) {
        oss << node->op << generate(node->operand.get(), className);
    } else {
        oss << generate(node->operand.get(), className) << node->op;
    }
    return oss.str();
}

std::string JavaCodeGenerator::generateTernaryExpr(const TernaryExpr* node, const std::string& className) const {
    std::ostringstream oss;
    oss << generate(node->condition.get(), className) << " ? "
        << generate(node->trueExpr.get(), className) << " : "
        << generate(node->falseExpr.get(), className);
    return oss.str();
}

std::string JavaCodeGenerator::generateFunctionCall(const FunctionCall* node, const std::string& className) const {
    std::ostringstream oss;
    oss << generate(node->callee.get(), className) << "(";
    for (size_t i = 0; i < node->arguments.size(); ++i) {
        oss << generate(node->arguments[i].get(), className);
        if (i + 1 < node->arguments.size()) oss << ", ";
    }
    oss << ")";
    return oss.str();
}


std::string JavaCodeGenerator::generateMemberAccess(const MemberAccess* node, const std::string& className) const {
    std::ostringstream oss;
    oss << generate(node->object.get(), className) << "." << node->memberName;
    return oss.str();
}

std::string JavaCodeGenerator::generateArrayAccess(const ArrayAccess* node, const std::string& className) const {
    std::string base = generate(node->arrayExpr.get(), className);
    std::string index = generate(node->indexExpr.get(), className);

    // Try to get the type from the symbol table if arrayExpr is an Identifier
    const ASTNode* typeNode = nullptr;
    if (node->arrayExpr->type == ASTNodeType::IDENTIFIER) {
        std::string varName = static_cast<const Identifier*>(node->arrayExpr.get())->name;
        auto it = symbolTable.find(varName);
        if (it != symbolTable.end()) {
            typeNode = it->second;
        }
    }

    bool isMap = false;
    if (typeNode && typeNode->type == ASTNodeType::TEMPLATE_TYPE) {
        const TemplateType* tt = static_cast<const TemplateType*>(typeNode);
        std::string javaType = mapCppTypeNameToJava(tt->baseTypeName, false);
        if (javaType == "HashMap" || javaType == "Map") {
            isMap = true;
        }
    }

    if (isMap) {
        return base + ".get(" + index + ")";
    } else {
        return base + "[" + index + "]";
    }
}

std::string JavaCodeGenerator::generateSwitchStmt(const SwitchStmt* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "switch (" << generate(node->condition.get(), className) << ") {\n";
    for (const auto& stmt : node->cases) {
        oss << generate(stmt.get(), className) << "\n";
    }
    oss << "}";
    return oss.str();
}

std::string JavaCodeGenerator::generateCaseStmt(const CaseStmt* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "case " << generate(node->value.get(), className) << ": ";
    for (const auto& stmt : node->statements) {
        oss << generate(stmt.get(), className) << " ";
    }
    oss << "break;";
    return oss.str();
}

std::string JavaCodeGenerator::generateDefaultStmt(const DefaultStmt* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "default: ";
    for (const auto& stmt : node->statements) {
        oss << generate(stmt.get(), className) << " ";
    }
    oss << "break;";
    return oss.str();
}

std::string JavaCodeGenerator::generateSortCall(const SortCall* node, const std::string& className) const {
    // Assume node->container is the container to sort
    // Java: Collections.sort(container);
    std::ostringstream oss;
    oss << "Collections.sort(" << generate(node->container.get(), className) << ")";
    return oss.str();
}

std::string JavaCodeGenerator::generateFindCall(const FindCall* node, const std::string& className) const {
    // Java: container.contains(value)
    std::ostringstream oss;
    oss << generate(node->container.get(), className) << ".contains(" << generate(node->value.get(), className) << ")";
    return oss.str();
}

std::string JavaCodeGenerator::generateAccumulateCall(const AccumulateCall* node, const std::string& className) const {
    // Java: (simulate accumulate using streams and range)
    // Note: Java does not have direct equivalents for C++ iterators, so this is a simplification.
    std::ostringstream oss;
    oss << "// Warning: accumulate(begin, end, init) mapped as stream().reduce(init, Integer::sum)\n";
    oss << generate(node->beginExpr.get(), className) << ".stream().reduce("
        << generate(node->initialValue.get(), className) << ", Integer::sum)";
    return oss.str();
}

std::string JavaCodeGenerator::generateVectorAccess(const VectorAccess* node, const std::string& className) const {
    std::ostringstream oss;
    oss << generate(node->vectorExpr.get(), className) << "." << node->method << "(";
    for (size_t i = 0; i < node->arguments.size(); ++i) {
        oss << generate(node->arguments[i].get(), className);
        if (i + 1 < node->arguments.size()) oss << ", ";
    }
    oss << ")";
    return oss.str();
}

std::string JavaCodeGenerator::generateCoutExpr(const CoutExpr* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "System.out.print(";
    for (size_t i = 0; i < node->outputValues.size(); ++i) {
        oss << this->generate(node->outputValues[i].get(), className);
        if (i + 1 < node->outputValues.size()) oss << " + ";
    }
    oss << ")";
    return oss.str();
}

std::string JavaCodeGenerator::generateCerrExpr(const CerrExpr* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "System.err.print(";
    for (size_t i = 0; i < node->errorOutputs.size(); ++i) {
        oss << this->generate(node->errorOutputs[i].get(), className);
        if (i + 1 < node->errorOutputs.size()) oss << " + ";
    }
    oss << ")";
    return oss.str();
}

std::string JavaCodeGenerator::generateCinExpr(const CinExpr* node, const std::string& className) const {
    std::ostringstream oss;
    // Example: assign input to each target variable
    for (size_t i = 0; i < node->inputTargets.size(); ++i) {
        oss << this->generate(node->inputTargets[i].get(), className)
            << " = new java.util.Scanner(System.in).next()";
        if (i + 1 < node->inputTargets.size()) oss << ";\n";
    }
    return oss.str();
}

std::string JavaCodeGenerator::generateGetlineCall(const GetlineCall* node, const std::string& className) const {
    // Java: targetVar = new Scanner(System.in).nextLine();
    std::ostringstream oss;
    oss << generate(node->targetVar.get(), className) << " = new java.util.Scanner(System.in).nextLine()";
    return oss.str();
}

std::string JavaCodeGenerator::generateMallocCall(const MallocCall* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "new " << mapTypeNodeToJava(node->elementType.get()) << "["
        << generate(node->sizeExpr.get(), className) << "]";
    return oss.str();
}

std::string JavaCodeGenerator::generateFreeCall(const FreeCall* /*node*/, const std::string& /*className*/) const {
    // Java has garbage collection; free is a no-op
    return "// free() ignored in Java (garbage collected)";
}

std::string JavaCodeGenerator::generateAbsCall(const AbsCall* node, const std::string& className) const {
    // Java: Math.abs(value)
    std::ostringstream oss;
    oss << "Math.abs(" << generate(node->valueExpr.get(), className) << ")";
    return oss.str();
}
    
std::string JavaCodeGenerator::generateTemplateClassDecl(const TemplateClassDecl* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "public class " << className << "<";
    for (size_t i = 0; i < node->templateParams.size(); ++i) {
        oss << node->templateParams[i]->name;
        if (i + 1 < node->templateParams.size()) oss << ", ";
    }
    oss << "> {\n";
    // Members
    for (const auto& member : node->members) {
        if (member->type == ASTNodeType::VAR_DECL) {
            oss << "    " << generateVarDecl(static_cast<const VarDecl*>(member.get())) << "\n";
        } else if (member->type == ASTNodeType::FUNCTION_DECL) {
            oss << "    " << generateFunctionDecl(static_cast<const FunctionDecl*>(member.get()), className) << "\n";
        }
    }
    oss << "}\n";
    return oss.str();
}
std::string JavaCodeGenerator::generateTemplateFunctionDecl(const TemplateFunctionDecl* node, const std::string& className) const {
    std::ostringstream oss;
    oss << "public static <";
    for (size_t i = 0; i < node->templateParams.size(); ++i) {
        oss << node->templateParams[i]->name;
        if (i + 1 < node->templateParams.size()) oss << ", ";
    }
    oss << "> " << mapTypeNodeToJava(node->returnType.get()) << " " << node->name << "(";
    for (size_t i = 0; i < node->parameters.size(); ++i) {
        const VarDecl* param = static_cast<const VarDecl*>(node->parameters[i].get());
        oss << mapTypeNodeToJava(param->type.get()) << " " << param->name;
        if (i + 1 < node->parameters.size()) oss << ", ";
    }
    oss << ")";
    if (node->body) {
        oss << " " << generate(node->body.get(), className);
    } else {
        oss << " {}";
    }
    return oss.str();
}
// ...existing code...
