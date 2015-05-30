#pragma once
#include "VM/Compiler/AST/ASTNode.h"

namespace Compiler {
  class BooleanNode : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(this); }
    void SetBoolean(bool boolean) { m_boolean = boolean; };
    bool GetBoolean() { return m_boolean; }
  private:
    bool m_boolean;
  };
}
