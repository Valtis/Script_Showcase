#include "VM/Compiler/Tokens/Token.h"

namespace Compiler {

  Token::Token(TokenType type, int32_t line, int32_t column) 
    : m_type(type), m_line(line), m_column(column) {

  }

  Token::~Token() { }


  std::string TokenName(TokenType type) {
    switch (type) {
    case TokenType::LPAREN:
      return "(";
    case TokenType::RPAREN:
      return ")";
    case TokenType::PLUS:
      return "+";
    case TokenType::MINUS:
      return "-";
    case TokenType::MULTIPLY:
      return "*";
    case TokenType::DIVIDE:
      return "/";
    case TokenType::EQUAL:
      return "=";
    case TokenType::GREATER_THAN:
      return ">";
    case TokenType::GREATER_OR_EQUAL_THAN:
      return ">=";
    case TokenType::LESS_THAN:
      return "<";
    case TokenType::LESS_OR_EQUAL_THAN:
      return "<=";
    case TokenType::IDENTIFIER:
      return "identifier";
    case TokenType::STRING:
      return "string";
    case TokenType::INTEGER_NUMBER:
      return "integer number";
    case TokenType::FLOAT_NUMBER:
      return "float number";
    case TokenType::DOUBLE_NUMBER:
      return "double number";
    case TokenType::STATICS:
      return "statics";
    case TokenType::FUNCTION:
      return "function declaration";
    case TokenType::LOCALS:
      return "locals";
    case TokenType::SET_VALUE:
      return "setvalue";
    case TokenType::IF:
      return "if";
    case TokenType::RETURN:
      return "return";
    case TokenType::INVOKE_NATIVE:
      return "invoke_native";
    default:
      return "Token string representation not found";
    }
  }
}