#include "VM/Compiler/AST/ASTNode.h"

namespace Compiler {
  ASTNode::~ASTNode() {

  }

  void ASTNode::AddChildren(std::shared_ptr<ASTNode> node) {
    m_children.push_back(node);
  }

  std::vector<ASTNode*> ASTNode::GetChildren() {
    std::vector<ASTNode *> children;

    for (auto &child : m_children) {
      children.push_back(child.get());
    }

    return children;
  }

  std::string ASTNode::GetPositionInfo()
  {
    return std::string("line ") + std::to_string(m_line) + " column " + std::to_string(m_column);
  }

}