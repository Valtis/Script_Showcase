#pragma once
#include "VM/Compiler/AST/ASTVisitor.h"
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
namespace Compiler {
  class ASTNode {
  public:
    virtual ~ASTNode();
    std::vector<ASTNode*> GetChildren();
    void SetLine(uint32_t line) { m_line = line;  }
    void SetColumn(uint32_t column) { m_column = column;  }

    void AddChild(std::shared_ptr<ASTNode> node);
    virtual void Accept(ASTVisitor &visitor) = 0;
    std::string GetPositionInfo();
  private:
    std::vector<std::shared_ptr<ASTNode>> m_children;
    uint32_t m_line;
    uint32_t m_column;

  };
}
