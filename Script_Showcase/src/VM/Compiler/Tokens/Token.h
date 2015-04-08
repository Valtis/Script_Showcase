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

  class Token {
  public:
    Token(TokenType type, int32_t line, int32_t column);
    virtual ~Token();
    TokenType GetType();
    virtual std::string ToString() = 0;
  private:
    TokenType m_type;
    int32_t m_line;
    int32_t m_column;
  };

}