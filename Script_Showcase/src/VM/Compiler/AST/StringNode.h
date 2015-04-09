#pragma once

#pragma once
#include "VM/Compiler/AST/ASTNode.h"
#include <string>

namespace Compiler {
  class StringNode : public ASTNode {
  public:
    void Visit(ASTVisitor &visitor) { visitor.Visit(this); }
    void SetValue(std::string value) { m_value = value; };
    std::string GetValue() { return m_value; }
  private:
    std::string m_value;
  };
}
