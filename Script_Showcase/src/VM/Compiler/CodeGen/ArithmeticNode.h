#pragma once
#include "VM/Compiler/AST/ASTNode.h"
#include "VM/Compiler/Tokens/TokenType.h"

namespace Compiler {
  class ArithmeticNode : public ASTNode {
  public:
    void SetType(TokenType type) { m_type = type; };
    TokenType GetType() { return m_type; };
  private:
    TokenType m_type;
  };
}
