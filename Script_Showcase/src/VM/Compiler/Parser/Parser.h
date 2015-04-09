#pragma once
#include "VM/Compiler/Tokens/Token.h"
#include <vector>
#include <memory>
namespace Compiler {
  class Parser {
  public:
    Parser(std::vector<std::unique_ptr<Token>> tokens);
    void Parse();

  private: 
    void ParseOptionalStaticsList();
    void ParseIdentifierList();
    void ParseFunction();
    void ParseStatements();
    void ParseStatement();

    void ParseSetValue();
    void ParseInvokeNative();
    void ParseArithmeticExpression(TokenType type);
    void ParseArgumentList();
    
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