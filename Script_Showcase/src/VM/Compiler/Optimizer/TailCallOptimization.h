#pragma once


class VMFunction;
class VMState;

/*
  Optimizes tail calls when function call is statically binded at compile time. Functions that are called indirectly
  cannot be optimized at this stage.
*/
namespace ByteCodeOptimizer {
  void OptimizeTailCalls(VMState &state);
}