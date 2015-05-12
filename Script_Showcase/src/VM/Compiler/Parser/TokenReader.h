#pragma once
#include "VM/Compiler/Tokens/Token.h"
#include <vector>
#include <memory>

namespace Compiler {
  class Token;

  class TokenReader {
  public:
    explicit TokenReader(std::vector<std::unique_ptr<Token>> tokens);
    TokenReader(const TokenReader &) = delete;
    TokenReader operator=(const TokenReader &) = delete;
    Token *Peek(); // peeks next token
    Token *Peek2(); // peeks token following the next one
    Token *Advance();
    Token *Expect(TokenType type);
    Token *ExpectOneOf(std::vector<TokenType> tokenTypes);
    bool HasTokens();

  private:

    std::vector<std::unique_ptr<Token>> m_tokens;
    size_t m_position;
  };

}