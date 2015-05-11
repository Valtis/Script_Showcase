#include "VM/Compiler/Optimizer/TailCallOptimization.h"
#include "VM/Core/VMState.h"
#include "VM/Core/ByteCode.h"
#include <unordered_map>
/*
Optimizes tail calls when function call is statically binded at compile time. Functions that are called indirectly
cannot be optimized at this stage.
*/

namespace ByteCodeOptimizer {
  
  bool IsTailCall(VMFunction &function, uint32_t byteCodePos);

  void OptimizeTailCalls(VMState &state) {
    auto &functions = state.GetMutableFunctions();
    std::unordered_map<ByteCode, int> byteCodeArgCount;
    // arg counts for byte code; some bytecode instrucions have their arguments encoded in the bytecode stream
    // and we need to skip these when iterating the bytecode
    byteCodeArgCount[ByteCode::PUSH_INTEGER] = 1;
    byteCodeArgCount[ByteCode::PUSH_FLOAT] = 1;
    byteCodeArgCount[ByteCode::PUSH_DOUBLE] = 2;
    byteCodeArgCount[ByteCode::PUSH_BOOLEAN] = 1;
    byteCodeArgCount[ByteCode::LOAD_STATIC_OBJECT] = 1;
    byteCodeArgCount[ByteCode::STORE_STATIC_OBJECT] = 1;
    byteCodeArgCount[ByteCode::LOAD_LOCAL] = 1;
    byteCodeArgCount[ByteCode::STORE_LOCAL] = 1;
    byteCodeArgCount[ByteCode::INVOKE_MANAGED] = 1;
    byteCodeArgCount[ByteCode::JUMP] = 1;
    byteCodeArgCount[ByteCode::JUMP_IF_FALSE] = 1;
    byteCodeArgCount[ByteCode::JUMP_IF_TRUE] = 1;
    byteCodeArgCount[ByteCode::JUMP_IF_NEGATIVE] = 1;
    byteCodeArgCount[ByteCode::JUMP_IF_POSITIVE] = 1;
    byteCodeArgCount[ByteCode::JUMP_IF_ZERO] = 1;
    byteCodeArgCount[ByteCode::PUSH_FUNCTION] = 1;



    for (auto &function : functions) {
      for (uint32_t i = 0; i < function.GetByteCodeCount(); ++i) {
        auto code = function.GetByteCode(i);
        if (code == ByteCode::INVOKE_MANAGED) {
          // get the next instruction
          ++i;
          auto functionID = static_cast<uint32_t>(function.GetByteCode(i));
          // check if recursive call
          if (functionID == state.GetFunctionID(function.GetName())) {
           
            if (IsTailCall(function, i + 1)) {
              // replace functionID with 0, replace InvokeManaged with Jump, thus jumping to beginning of the function instead of performing function call

              function.ChangeByteCode(i, static_cast<ByteCode>(0));
              function.ChangeByteCode(i - 1, ByteCode::JUMP);

            }
          }
        }

        // skip any arguments. Relies on the fact that []-operator returns default value (0) 
        // on any instruction that is not listed above.
        i += byteCodeArgCount[code];
      }
    }
  }

  // checks if next instruction is return, thus making this tail call
  // alternatively, if instruction is unconditional jump, check the pointed instruction until non-jump is found. Then check if this instruction is return
  bool IsTailCall(VMFunction &function, uint32_t byteCodePos) {
    if (function.GetByteCode(byteCodePos) == ByteCode::RETURN) {
      return true;
    }

    if (function.GetByteCode(byteCodePos) == ByteCode::JUMP) {
      auto targetInstruction = static_cast<uint32_t>(function.GetByteCode(byteCodePos + 1));

      return IsTailCall(function, targetInstruction);
    }

    return false;
  }
}