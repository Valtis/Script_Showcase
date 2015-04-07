#include "VM/Core/VMOperations.h"
#include "VM/Core/VMValue.h"
#include "VM/Core/VMFrame.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMFunction.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/FFI/NativeBinding.h"
#include "VM/Memory/MemoryManager.h"
#include "VM/Utility/LoggerManager.h"
#include <vector>

namespace Op {

  void PushValue(const VMValue &value, std::vector<VMValue> &stack) {
    stack.push_back(value);
  }

  VMValue PopValue(std::vector<VMValue> &stack) {
    auto value = stack.back();
    stack.pop_back();
    return value;
  }

  void PushInteger(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    PushValue(VMValue{ static_cast<int32_t>(frames.back().GetNextInstruction()) }, stack);
  }


  void LoadStaticObject(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = static_cast<uint32_t>(frames.back().GetNextInstruction());
    PushValue(state.GetStaticObject(index), stack);
  }

  void StoreStaticObject(VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);
    state.SetStaticObject(index, value);
  }

  void LoadArrayIndex(std::vector<VMValue> &stack) {
    auto array = PopValue(stack);
    auto index = PopValue(stack);
    auto type = MemMgrInstance().GetArrayType(array);
    
    VMValue value{ type };
    MemMgrInstance().ReadFromArrayIndex(array, value.value_pointer(), index.as_int(), 1);
    PushValue(value, stack);
  }

  void StoreArrayIndex(std::vector<VMValue> &stack) {
    auto array = PopValue(stack);
    auto index = PopValue(stack);
    auto value = PopValue(stack);
    auto type = MemMgrInstance().GetArrayType(array);
    if (type != value.type()) {
      throw std::runtime_error("Array type and value type mismatch: Array is of type " + TypeToString(type) + " and value is of type " + 
        TypeToString(value.type()));
    }

    MemMgrInstance().WriteToArrayIndex(array, value.value_pointer(), index.as_int(), 1);
  }

  void AddInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(VMValue{ first + second }, stack);
  }

  void SubInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(VMValue{ first - second }, stack);
  }

  void MulInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(VMValue{ first * second }, stack);
  }

  void DivInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(VMValue{ first / second }, stack);
  }

  void InvokeNative(const VMState &state, std::vector<VMValue> &stack) {
    auto ptrToStr = PopValue(stack);
    auto binding = state.GetNativeBinding(ToNativeType<std::string>(ptrToStr));
    binding(stack);
  }

  void InvokeManaged(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto function = state.GetFunction(index);
    frames.push_back(function);
  }


  bool Return(std::vector<VMFrame> &frames) {
    frames.pop_back();
    if (frames.empty())  {
      return false;
    }
    return true;

  }

  void JumpIfZero(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);

    if (value.as_int() == 0) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }

  void JumpIfNegative(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);
    if (value.as_int() < 0) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }

  void JumpIfPositive(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);

    if (value.as_int() > 0) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }

  void StoreLocal(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);

    frames.back().SetLocalVariable(index, value);
  }

  void LoadLocal(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = static_cast<uint32_t>(frames.back().GetNextInstruction());
    PushValue(frames.back().GetLocalVariable(index), stack);
  }

  void DoubleToInteger(std::vector<VMValue> &stack) {
    auto value = PopValue(stack);
    value.set_int(static_cast<int32_t>(value.as_double()));
    PushValue(value, stack);
  }

  void AllocateIntegerArray(std::vector<VMValue> &stack) {
    auto size = PopValue(stack);
    PushValue(MemMgrInstance().AllocateArray(ValueType::INT, size.as_int()), stack);
  }

  void AllocateObjectArray(std::vector<VMValue> &stack) {
    auto size = PopValue(stack);
    PushValue(MemMgrInstance().AllocateArray(ValueType::MANAGED_POINTER, size.as_int()), stack);
  }

}