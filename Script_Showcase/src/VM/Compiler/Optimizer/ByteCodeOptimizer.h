#pragma once

/*
  these functions will optimize the bytecode. Currently, only tail call optimization is implemented.
*/

class VMState;

namespace ByteCodeOptimizer {
  void OptimizeByteCode(VMState &state);
}