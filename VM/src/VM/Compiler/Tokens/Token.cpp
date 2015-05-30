#include "VM/Compiler/Tokens/Token.h"

namespace Compiler {

  Token::Token(TokenType type, int32_t line, int32_t column) 
    : m_type(type), m_line(line), m_column(column) {

  }

  Token::~Token() { }

  std::string Token::GetTokenPositionInfo() {
    return std::string("line ") + std::to_string(m_line)
      + " column " + std::to_string(m_column);
  }
}