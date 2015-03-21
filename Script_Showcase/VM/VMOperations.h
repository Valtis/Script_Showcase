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
  void PushConstantObject(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
 
  void AddInteger(std::vector<VMValue> &stack);
  void SubInteger(std::vector<VMValue> &stack);
  void MulInteger(std::vector<VMValue> &stack);
  void DivInteger(std::vector<VMValue> &stack);

  void InvokeNative(const VMState &state, std::vector<VMValue> &stack);
  void InvokeManaged(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames);
  

  // returns true if there are still frames remaining after returning
  bool Return(std::vector<VMFrame> &frames);
}