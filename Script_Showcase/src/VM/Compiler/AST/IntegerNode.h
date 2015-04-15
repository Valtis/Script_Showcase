#pragma once
#include "VM/Compiler/AST/ASTNode.h"
#include <cstdint>
namespace Compiler {
  class IntegerNode : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override  { visitor.Visit(this); }
    void SetNumber(int32_t number) { m_number = number ; };
    int32_t GetNumber() { return m_number; }
  private:
    int32_t m_number;
  };
}
