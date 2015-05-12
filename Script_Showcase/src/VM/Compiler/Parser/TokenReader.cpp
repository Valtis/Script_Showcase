#include "VM/Compiler/Parser/TokenReader.h"
namespace Compiler {
  TokenReader::TokenReader(std::vector<std::unique_ptr<Token>> tokens) : m_tokens(std::move(tokens)) {
  }



  Token *TokenReader::Peek() {
    if (m_position >= m_tokens.size()) {
      return nullptr;
    }
    return m_tokens[m_position].get();
  }

  Token *TokenReader::Peek2() {
    if (m_position + 1 >= m_tokens.size()) {
      return nullptr;
    }
    return m_tokens[m_position + 1].get();
  }

  Token *TokenReader::Advance() {
    if (m_position >= m_tokens.size()) {
      throw std::runtime_error("Unexpected end-of-file while parsing");
    }

    return m_tokens[m_position++].get();
  }

  Token *TokenReader::Expect(TokenType type) {
    Token *actual;
    try {
      actual = Advance();
    }
    catch (std::exception &ex) {
      throw std::runtime_error(std::string(ex.what()) + ". Expected token '" + TokenName(type) + "'");
    }
    if (actual->GetType() != type) {
      auto expected = TokenName(type);
      throw std::runtime_error("Unexpected token '" + actual->ToString() + "' at "
        + actual->GetTokenPositionInfo() + ". Token '" + expected + "' was expected");
    }
    return actual;
  }

  Token *TokenReader::ExpectOneOf(std::vector<TokenType> tokenTypes) {
    auto actual = Advance();

    if (std::find(tokenTypes.begin(), tokenTypes.end(), actual->GetType()) == tokenTypes.end()) {
      std::string tokenList = "";
      for (auto type : tokenTypes) {
        tokenList += " '" + TokenName(type) + "'";
      }
      throw std::runtime_error("Unexpected token '" + actual->ToString() + "' at " +
        actual->GetTokenPositionInfo() + ". Expected one of" + tokenList);
    }
    return actual;
  }

  bool TokenReader::HasTokens() {
    return m_position != m_tokens.size();
  }
}