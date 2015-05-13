#include "VM/Compiler/Tokens/TokenType.h"

namespace Compiler {
  
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
    case TokenType::MOD:
      return "mod";
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
    case TokenType::TRUE:
      return "true";
    case TokenType::FALSE:
      return "false";
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
    case TokenType::ELSE:
      return "else";
    case TokenType::COND:
      return "cond";
    case TokenType::AND:
      return "and";
    case TokenType::OR:
      return "or";
    case TokenType::NOT:
      return "not";
    case TokenType::WHILE:
      return "while";
    case TokenType::RETURN:
      return "return";
    case TokenType::INVOKE_NATIVE:
      return "invokenative";
    case TokenType::INTEGER_ARRAY:
      return "integerarray";
    case TokenType::WRITE_ARRAY:
      return "writearray";
    case TokenType::READ_ARRAY:
      return "readarray";
    case TokenType::ARRAY_LENGTH:
      return "arraylength";
    default:
      return "Token string representation not found";
    }
  }
}