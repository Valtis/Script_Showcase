#pragma once
#include "VM/Compiler/AST/ASTVisitor.h"
#include "VM/Core/VMState.h"
namespace Compiler {
  class CodeGeneratorVisitor : public ASTVisitor {
  public:

    VMState AcquireState();
  private:
    VMState m_state;
  };
}