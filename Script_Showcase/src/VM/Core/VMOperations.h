#pragma once
#include <cstdint>
#include <vector>
class VMState;
class VMFrame;
class VMValue;

namespace Op {
  void PushValue(const VMValue &value, std::vector<VMValue> &stack);
  VMValue PopValue(std::vector<VMValue> &stack);
  void PushInteger(std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  
  void LoadStaticObject(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void StoreStaticObject(VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);

  void LoadArrayIndex(std::vector<VMValue> &stack);
  void StoreArrayIndex(std::vector<VMValue> &stack);

  void AddInteger(std::vector<VMValue> &stack);
  void SubInteger(std::vector<VMValue> &stack);
  void MulInteger(std::vector<VMValue> &stack);
  void DivInteger(std::vector<VMValue> &stack);

  void InvokeNative(const VMState &state, std::vector<VMValue> &stack);
  void InvokeManaged(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  
  void JumpIfZero(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void JumpIfNegative(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void JumpIfPositive(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);


  void StoreLocal(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  void LoadLocal(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);

  void DoubleToInteger(std::vector<VMValue> &stack);

  void AllocateIntegerArray(std::vector<VMValue> &stack);
  void AllocateObjectArray(std::vector<VMValue> &stack);
  
  // returns true if there are still frames remaining after returning
  bool Return(std::vector<VMFrame> &frames);
}