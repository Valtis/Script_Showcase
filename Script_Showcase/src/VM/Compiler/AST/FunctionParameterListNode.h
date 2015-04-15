#pragma once
#include "VM/Compiler/AST/ASTNode.h"

namespace Compiler {
  class FunctionParameterListNode : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(this); }
  private:
  };
}
