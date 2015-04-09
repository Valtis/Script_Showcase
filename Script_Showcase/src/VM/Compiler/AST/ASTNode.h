#pragma once
#include "VM/Compiler/AST/ASTVisitor.h"

namespace Compiler {
  class ASTNode {
  public:
    virtual ~ASTNode();
    virtual void Accept(ASTVisitor &visitor) = 0;
  };
}
