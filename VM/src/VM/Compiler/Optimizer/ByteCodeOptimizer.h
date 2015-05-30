#pragma once

/*
  Applies optimizations to the bytecode. 
*/

class VMState;
namespace ByteCodeOptimizer {
  void OptimizeByteCode(VMState &state);
}