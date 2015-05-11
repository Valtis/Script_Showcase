#pragma once


class VMFunction;
class VMState;

namespace ByteCodeOptimizer {
  void OptimizeTailCalls(VMState &state);
}