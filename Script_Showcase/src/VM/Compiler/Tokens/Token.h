#pragma once
#include "VM/Compiler/Tokens/TokenType.h"
#include <string>
#include <cstdint>

namespace Compiler {
  /*
    A base class that represents a token.
  */
  class Token {
  public:
    Token(TokenType type, int32_t line, int32_t column);
    virtual ~Token();
    TokenType GetType() const { return m_type; }
    virtual std::string ToString() const = 0;
    int GetLine() const { return m_line; }
    int GetColumn() const { return m_column;  }
  private:
    TokenType m_type;
    int32_t m_line;
    int32_t m_column;
  };

}