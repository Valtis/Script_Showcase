#pragma once
#include "VM/Core/VMValue.h"
#include "VM/Exceptions/Exceptions.h"
#include <vector>

class VMState;
class VMFrame;
class VMValue;

const uint32_t STACK_SIZE = 4096;
const uint32_t FRAME_SIZE = 1024;
/*
  Functions that actually perform the work required by the VM opcodes. 
*/
namespace Op {
  void PushValue(const VMValue &value, std::vector<VMValue> &stack);
  VMValue PopValue(std::vector<VMValue> &stack);
  void PushInteger(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void PushFloat(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void PushFunction(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void PushDouble(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void PushBoolean(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);

  void StoreLocal(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void LoadLocal(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);

  void LoadStaticObject(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void StoreStaticObject(VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);

  void LoadArrayIndex(std::vector<VMValue> &stack);
  void StoreArrayIndex(std::vector<VMValue> &stack);
  void ArrayLength(std::vector<VMValue> &stack);
  
  void AddInteger(std::vector<VMValue> &stack);
  void SubInteger(std::vector<VMValue> &stack);
  void MulInteger(std::vector<VMValue> &stack);
  void DivInteger(std::vector<VMValue> &stack);
  

  // these functions will do type conversions automatically
  void Add(std::vector<VMValue> &stack);
  void Sub(std::vector<VMValue> &stack);
  void Mul(std::vector<VMValue> &stack);
  void Div(std::vector<VMValue> &stack);
  void Mod(std::vector<VMValue> &stack);
  void Not(std::vector<VMValue> &stack);

  void InvokeNative(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void InvokeManaged(const VMState &state, std::vector<VMFrame> &frames);
  void InvokeManagedIndirect(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);


  // these functions will do type conversions automatically
  void IsGreater(std::vector<VMValue> &stack);
  void IsGreaterOrEq(std::vector<VMValue> &stack);
  void IsEq(std::vector<VMValue> &stack);
  void IsLessOrEq(std::vector<VMValue> &stack);
  void IsLess(std::vector<VMValue> &stack);


  void Jump(std::vector<VMFrame> &frames);
  void JumpIfTrue(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void JumpIfFalse(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);

  void AllocateIntegerArray(std::vector<VMValue> &stack);
  void AllocateObjectArray(std::vector<VMValue> &stack);
  
  // returns true if there are still frames remaining after returning
  bool Return(std::vector<VMFrame> &frames);
}