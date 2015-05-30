#pragma once
#include "VM/Compiler/Tokens/Token.h"
#include <vector>
#include <memory>

namespace Compiler {
  class Token;
  /*
    Helper class used by parser. Provides Peek\Expect etc. functions that parser uses.
  */

  class TokenReader {
  public:
    explicit TokenReader(std::vector<std::unique_ptr<Token>> tokens);
    TokenReader(const TokenReader &) = delete;
    TokenReader operator=(const TokenReader &) = delete;
    // peeks next token. Returns null if no token exists.
    Token *Peek(); 
    // peeks token following the next one. Returns null if no token exists. (WTB option type)
    Token *Peek2(); 
    // returns next token. Throws an exception if no token exists.
    Token *Advance(); 
    // returns token if the type matches or throws an exception.
    Token *Expect(TokenType type); 
    // Returns a token if the type matches one of the given types or throws an exception
    Token *ExpectOneOf(std::vector<TokenType> tokenTypes); 
    bool HasTokens();

  private:

    std::vector<std::unique_ptr<Token>> m_tokens;
    size_t m_position;
  };

}