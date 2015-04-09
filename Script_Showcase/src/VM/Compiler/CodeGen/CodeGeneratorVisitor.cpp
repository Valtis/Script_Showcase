#include "VM/Compiler/CodeGen/CodeGeneratorVisitor.h"
#include "VM/Compiler/AST/ArithmeticNode.h"

#include <iostream> // can be removed - debugging only
namespace Compiler {

  VMState CodeGeneratorVisitor::AcquireState() {
    return std::move(m_state);
  }

  void CodeGeneratorVisitor::Visit(ArithmeticNode *node)
  {
    std::cout << "ArithNode\n";
  }
}
