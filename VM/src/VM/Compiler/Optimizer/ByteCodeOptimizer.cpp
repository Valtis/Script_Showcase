#include "VM/Compiler/Optimizer/ByteCodeOptimizer.h"
#include "VM/Compiler/Optimizer/TailCallOptimization.h"
#include "VM/Core/VMState.h"

namespace ByteCodeOptimizer {

  void OptimizeByteCode(VMState &state) {
    OptimizeTailCalls(state);
  }

}
