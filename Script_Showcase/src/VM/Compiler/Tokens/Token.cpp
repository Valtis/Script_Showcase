#include "VM/Compiler/Tokens/Token.h"

namespace Compiler {

  Token::Token(TokenType type, int32_t line, int32_t column) 
    : m_type(type), m_line(line), m_column(column) {

  }

  Token::~Token() { }

  TokenType Token::GetType() {
    return m_type;
  }

}