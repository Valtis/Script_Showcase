#include "VM/Compiler/Parser/Parser.h"
#include "VM/Compiler/Tokens/TokenFactory.h"
namespace Compiler {
  Parser::Parser(std::vector<std::unique_ptr<Token>> tokens) 
    : m_tokens(std::move(tokens)), m_position(0) {

  }

  void Parser::Parse() {

    ParseOptionalStaticsList();
    while (true) {
      if (m_position == m_tokens.size()) {
        break;
      }
      ParseFunction();
    }
  }

  void Parser::ParseOptionalStaticsList() {
    auto token = Peek2();
    if (token && token->GetType() == TokenType::STATICS) {
      Expect(TokenType::LPAREN);
      Expect(TokenType::STATICS);
      ParseIdentifierList();
      Expect(TokenType::RPAREN);
    }
  }

  void Parser::ParseIdentifierList() {
    while (Peek()->GetType() == TokenType::IDENTIFIER) {
      Advance();
    }
  }

  void Parser::ParseFunction() {
    Expect(TokenType::LPAREN);
    Expect(TokenType::FUNCTION);
    Expect(TokenType::IDENTIFIER);
    Expect(TokenType::LPAREN);
    ParseIdentifierList();
    Expect(TokenType::RPAREN);
    Expect(TokenType::LPAREN);
    ParseStatements();
    Expect(TokenType::RPAREN);
    Expect(TokenType::RPAREN);
  }

  void Parser::ParseStatements() {

  }


  Token *Parser::Peek() {
    if (m_position >= m_tokens.size()) {
      return nullptr;
    }
    return m_tokens[m_position].get();
  }

  Token *Parser::Peek2() {
    if (m_position + 1 >= m_tokens.size()) {
      return nullptr;
    }
    return m_tokens[m_position+1].get();
  }

  Token *Parser::Advance() {
    return m_tokens[m_position++].get();
  }

  Token *Parser::Expect(TokenType type) {
    if (m_tokens[m_position]->GetType() != type) {
      auto expected = "TODO-WRITE-TOKENS-TO-STRING. TokenID: " + std::to_string(static_cast<int>(type));
      auto actual = m_tokens[m_position].get();
      throw std::runtime_error("Invalid token " + actual->ToString() + " at line " + std::to_string(actual->GetLine())
        + " column " +  std::to_string(actual->GetColumn()) + ". Token " + expected + " was expected");
    }

    return Advance();
  }
}