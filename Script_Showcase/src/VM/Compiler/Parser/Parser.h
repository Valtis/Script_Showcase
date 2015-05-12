#pragma once
#include "VM/Compiler/Tokens/Token.h"
#include "VM/Compiler/Parser/TokenReader.h"
#include <vector>
#include <memory>
namespace Compiler {
  class ASTNode;

  /*
    Goes through the list of tokens, using TokenReader. If there are syntax errors, throws. Otherwise creates and returns abstract syntax tree
    Grammar is available in separate file
  */
  class Parser {
  public:
    explicit Parser(std::vector<std::unique_ptr<Token>> tokens);
    std::shared_ptr<ASTNode> Parse();

  private: 
    void ParseOptionalStaticsList(std::shared_ptr<ASTNode> parent);
    void ParseIdentifierList(std::shared_ptr<ASTNode> parent);
    void ParseFunction(std::shared_ptr<ASTNode> parent);
    void ParseLocals(std::shared_ptr<ASTNode> parent);
    void ParseStatements(std::shared_ptr<ASTNode> parent);
    void ParseStatement(std::shared_ptr<ASTNode> parent);
    void ParseReturn(std::shared_ptr<ASTNode> parent);
    void ParseIf(std::shared_ptr<ASTNode> parent);
    void ParseCond(std::shared_ptr<ASTNode> parent);
    void ParseWhile(std::shared_ptr<ASTNode> parent);
    void ParseExpression(std::shared_ptr<ASTNode> parent);
    void ParseSetValue(std::shared_ptr<ASTNode> parent);
    void ParseInvokeNative(std::shared_ptr<ASTNode> parent);
    void ParseArithmeticExpression(std::shared_ptr<ASTNode> parent);
    void ParseComparisonExpression(std::shared_ptr<ASTNode> parent);
    void ParseAndOrExpression(std::shared_ptr<ASTNode> parent);
    void ParseFunctionCall(std::shared_ptr<ASTNode> parent);
    void ParseArgumentList(std::shared_ptr<ASTNode> parent);
    void CreateIdentifierNode(std::shared_ptr<ASTNode> parent, Token* token);
    void ParseAllocateArray(std::shared_ptr<ASTNode> parent);
    void ParseWriteArray(std::shared_ptr<ASTNode> parent);
    void ParseReadArray(std::shared_ptr<ASTNode> parent);
    void ParseArrayLength(std::shared_ptr<ASTNode> parent);


    void ParseLiteralOrIdentifier(std::shared_ptr<ASTNode> parent);


    TokenReader m_reader;
  };
}