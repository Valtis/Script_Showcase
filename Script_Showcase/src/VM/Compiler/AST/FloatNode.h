#pragma once
#include "VM/Compiler/AST/ASTNode.h"

namespace Compiler {
  class FloatNode : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(this); }
    void SetNumber(float number) { m_number = number; };
    float GetNumber() { return m_number; }
  private:
    float m_number;
  };
}
