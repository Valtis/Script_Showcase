#pragma once 
#include <string>
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
}