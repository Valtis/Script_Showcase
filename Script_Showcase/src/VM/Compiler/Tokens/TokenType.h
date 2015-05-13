#pragma once 
#include <string>
namespace Compiler {
  /*
    Token type enum.
  */
  enum class TokenType {
    LPAREN,
    RPAREN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MOD,
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
    TRUE,
    FALSE,
    STATICS,
    FUNCTION,
    LOCALS,
    SET_VALUE,
    IF,
    COND,
    ELSE,
    AND,
    OR, 
    NOT,
    WHILE,
    RETURN,
    INVOKE_NATIVE,
    INTEGER_ARRAY,
    READ_ARRAY,
    WRITE_ARRAY,
    ARRAY_LENGTH
  };

  std::string TokenName(TokenType type);
}