#include "VM/Compiler/CodeGen/CodeGeneratorVisitor.h"

namespace Compiler {
  VMState CodeGeneratorVisitor::AcquireState() {
    return std::move(m_state);
  }
}
