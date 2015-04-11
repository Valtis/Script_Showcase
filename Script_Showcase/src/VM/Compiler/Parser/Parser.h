#pragma once
#include "VM/Compiler/Tokens/Token.h"
#include <vector>
#include <memory>
namespace Compiler {
  class ASTNode;
  class Parser {
  public:
    Parser(std::vector<std::unique_ptr<Token>> tokens);
    std::shared_ptr<ASTNode> Parse();

  private: 
    void ParseOptionalStaticsList(std::shared_ptr<ASTNode> parent);
    void ParseIdentifierList(std::shared_ptr<ASTNode> parent);
    void ParseFunction(std::shared_ptr<ASTNode> parent);
    void ParseLocals(std::shared_ptr<ASTNode> parent);
    void ParseStatements(std::shared_ptr<ASTNode> parent);
    void ParseStatement(std::shared_ptr<ASTNode> parent);
    void ParseIf(std::shared_ptr<ASTNode> parent);
    void ParseExpression(std::shared_ptr<ASTNode> parent);
    void ParseSetValue(std::shared_ptr<ASTNode> parent);
    void ParseInvokeNative(std::shared_ptr<ASTNode> parent);
    void ParseArithmeticExpression(std::shared_ptr<ASTNode> parent);
    void ParseComparisonExpression(std::shared_ptr<ASTNode> parent);
    void ParseArgumentList(std::shared_ptr<ASTNode> parent);

    void ParseLiteralOrIdentifier(std::shared_ptr<ASTNode> parent);
    
    std::vector<std::unique_ptr<Token>> m_tokens;
    size_t m_position;

    Token *Peek(); // peeks next token
    Token *Peek2(); // peeks token following the next one
    Token *Advance();
    Token *Expect(TokenType type);
    Token *ExpectOneOf(std::vector<TokenType> tokenTypes);

    std::string GetTokenPositionInfo(const Token *token);
    
  };
}