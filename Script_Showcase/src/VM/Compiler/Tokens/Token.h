#pragma once

#include <string>
#include <cstdint>

namespace Compiler {

  enum class TokenType {
    LPAREN, 
    RPAREN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EQUAL,
    GREATER_THAN,
    GREATER_OR_EQUAL_THAN,
    LESS_THAN,
    LESS_OR_EQUAL_THAN,
    IDENTIFIER,
    STRING,
    INTEGER_NUMBER,
    FLOAT_NUMBER,
    DOUBLE_NUMBER,
    STATICS,
    FUNCTION,
    LOCALS,
    SET_VALUE,
    IF,
    RETURN,
    INVOKE_NATIVE,
  };

  std::string TokenName(TokenType type);

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