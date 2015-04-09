#pragma once
#include "VM/Compiler/AST/ASTVisitor.h"
#include "VM/Core/VMState.h"

namespace Compiler {
  class CodeGeneratorVisitor : public ASTVisitor {
  public:

    VMState AcquireState();
    void Visit(ArithmeticNode *node) override;

  private:
    VMState m_state;
  };
}