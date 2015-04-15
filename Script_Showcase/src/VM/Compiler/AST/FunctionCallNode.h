#pragma once
#include "VM/Compiler/AST/ASTNode.h"

namespace Compiler {
  class FunctionCallNode : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(this); }
    void SetName(std::string name) { m_name = name;  }
    std::string GetName() { return m_name;  }
  private:
    std::string m_name;
  };
}
