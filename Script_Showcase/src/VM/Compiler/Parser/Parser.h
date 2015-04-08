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


    std::vector<std::unique_ptr<Token>> m_tokens;
    int m_position;

    Token *Peek(); // peeks next token
    Token *Peek2(); // peeks token following the next one
    Token *Advance();
    Token *Expect(TokenType type);
  };
}