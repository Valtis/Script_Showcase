#pragma once
#include "VM/Compiler/AST/ASTNode.h"

#include <string>
namespace Compiler {
  class FunctionNode : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(this); }
    void SetName(std::string name) { m_name = name; }
    std::string GetName() { return m_name; }
  private:
    std::string m_name;
  };
}
