#pragma once
#include "VM/Compiler/AST/ASTNode.h"

namespace Compiler {
  class StaticsNode : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(this); }
  private:
  };
}
