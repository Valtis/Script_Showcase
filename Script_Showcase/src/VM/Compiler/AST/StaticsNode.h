#pragma once
#include "VM/Compiler/AST/ASTNode.h"

namespace Compiler {
  class StaticsNode : public ASTNode {
  public:
    void Visit(ASTVisitor &visitor) { visitor.Visit(this); }
  private:
  };
}
