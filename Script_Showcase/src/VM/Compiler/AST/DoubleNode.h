#pragma once
#include "VM/Compiler/AST/ASTNode.h"

namespace Compiler {
  class DoubleNode : public ASTNode {
  public:
    void Visit(ASTVisitor &visitor) { visitor.Visit(this); }
    void SetNumber(double number) { m_number = number; };
    double GetNumber() { return m_number; }
  private:
    double m_number;
  };
}
