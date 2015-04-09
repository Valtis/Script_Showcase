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
    if (stack.size() == stackSize) {
      throw std::runtime_error("Stack overflow");
    }
    stack.push_back(value);
  }

  VMValue PopValue(std::vector<VMValue> &stack) {
    if (stack.size() == 0) {
      throw std::runtime_error("Stack underflow");
    }
    auto value = stack.back();
    stack.pop_back();
    return value;
  }

  void PushInteger(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    PushValue(VMValue{ static_cast<int32_t>(frames.back().GetNextInstruction()) }, stack);
  }
  void PushFloat(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    uint32_t val = static_cast<uint32_t>(frames.back().GetNextInstruction());
    PushValue(VMValue{ *reinterpret_cast<float*>(&val) }, stack);
  } 
  
  void PushDouble(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    // here be dragons
    uint32_t highbits = static_cast<uint32_t>(frames.back().GetNextInstruction());
    uint32_t lowbits = static_cast<uint32_t>(frames.back().GetNextInstruction());
    
    uint64_t val = 0;

    char *ptr = (char *)&val;
    char *highPtr = (char *)&highbits;
    char *lowPtr = (char *)&lowbits;

    ptr[7] = highPtr[3];
    ptr[6] = highPtr[2];
    ptr[5] = highPtr[1];
    ptr[4] = highPtr[0];
    ptr[3] = lowPtr[3];
    ptr[2] = lowPtr[2];
    ptr[1] = lowPtr[1];
    ptr[0] = lowPtr[0];

    auto doubleVal = *reinterpret_cast<double *>(&val);
    PushValue(VMValue{ doubleVal }, stack);
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
    MemMgrInstance().ReadFromArrayIndex(array, value.ValuePointer(), index.AsInt(), 1);
    PushValue(value, stack);
  }

  void StoreArrayIndex(std::vector<VMValue> &stack) {
    auto array = PopValue(stack);
    auto index = PopValue(stack);
    auto value = PopValue(stack);
    auto type = MemMgrInstance().GetArrayType(array);
    if (type != value.GetType()) {
      throw std::runtime_error("Array type and value type mismatch: Array is of type " + TypeToString(type) + " and value is of type " + 
        TypeToString(value.GetType()));
    }

    MemMgrInstance().WriteToArrayIndex(array, value.ValuePointer(), index.AsInt(), 1);
  }

  void AddInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).AsInt();
    auto first = PopValue(stack).AsInt();
    PushValue(VMValue{ first + second }, stack);
  }

  void SubInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).AsInt();
    auto first = PopValue(stack).AsInt();
    PushValue(VMValue{ first - second }, stack);
  }

  void MulInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).AsInt();
    auto first = PopValue(stack).AsInt();
    PushValue(VMValue{ first * second }, stack);
  }

  void DivInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).AsInt();
    auto first = PopValue(stack).AsInt();
    PushValue(VMValue{ first / second }, stack);
  }

  // TODO: REFACTOR

  ValueType GetConversionType(ValueType first, ValueType second) {
    
    if (first == ValueType::DOUBLE || second == ValueType::DOUBLE) {
      return ValueType::DOUBLE;
    }
    
    if (first == ValueType::FLOAT || second == ValueType::FLOAT) {
      return ValueType::FLOAT;
    }
    return ValueType::INT;
  }

  VMValue ConvertToType(ValueType type, VMValue value) {

    if (value.GetType() == type) {
      return value;
    }

    switch (type) {
    case ValueType::DOUBLE:
    
      if (value.GetType() == ValueType::FLOAT) {
        return VMValue{ (double)value.AsFloat() };
      } else if (value.GetType() == ValueType::INT) {
        return VMValue{ (double)value.AsInt() };
      }
      throw std::runtime_error("TypeError: Could not convert " + TypeToString(value.GetType()) + " to " + TypeToString(type));
    
    break;
    case ValueType::FLOAT:
    
      if (value.GetType() == ValueType::DOUBLE) {
        return VMValue{ (float)value.AsDouble() };
      }
      else if (value.GetType() == ValueType::INT) {
        return VMValue{ (float)value.AsInt() };
      }
      throw std::runtime_error("TypeError: Could not convert " + TypeToString(value.GetType()) + " to " + TypeToString(type));
    case ValueType::INT:

      if (value.GetType() == ValueType::DOUBLE) {
        return VMValue{ (int32_t)value.AsDouble() };
      }
      else if (value.GetType() == ValueType::FLOAT) {
        return VMValue{ (float)value.AsFloat() };
      }
      throw std::runtime_error("TypeError: Could not convert " + TypeToString(value.GetType()) + " to " + TypeToString(type));

    default:
      throw std::runtime_error("Invalid type for type conversion: " + TypeToString(type));
    }
  }

  void Add(std::vector<VMValue> &stack) {
    auto second = PopValue(stack);
    auto first = PopValue(stack);
    auto type = GetConversionType(first.GetType(), second.GetType());

    auto firstConverted = ConvertToType(type, first);
    auto secondConverted = ConvertToType(type, second);

    if (type ==  ValueType::DOUBLE) {
      PushValue(VMValue{ firstConverted.AsDouble() + secondConverted.AsDouble() }, stack);
    } else if (type == ValueType::FLOAT) {
      PushValue(VMValue{ firstConverted.AsFloat() + secondConverted.AsFloat() }, stack);
    } else if (type == ValueType::INT) {

      PushValue(VMValue{ firstConverted.AsInt() + secondConverted.AsInt() }, stack);
    } else {
      throw std::runtime_error("Invalid type for addition: " + TypeToString(type));
    }
  } 
  
  void Sub(std::vector<VMValue> &stack) {
    auto second = PopValue(stack);
    auto first = PopValue(stack);
    auto type = GetConversionType(first.GetType(), second.GetType());

    auto firstConverted = ConvertToType(type, first);
    auto secondConverted = ConvertToType(type, second);

    if (type == ValueType::DOUBLE) {
      PushValue(VMValue{ firstConverted.AsDouble() - secondConverted.AsDouble() }, stack);
    }
    else if (type == ValueType::FLOAT) {
      PushValue(VMValue{ firstConverted.AsFloat() - secondConverted.AsFloat() }, stack);
    }
    else if (type == ValueType::INT) {

      PushValue(VMValue{ firstConverted.AsInt() - secondConverted.AsInt() }, stack);
    }
    else {
      throw std::runtime_error("Invalid type for subtraction: " + TypeToString(type));
    }
  }

  void Mul(std::vector<VMValue> &stack) {
    auto second = PopValue(stack);
    auto first = PopValue(stack);
    auto type = GetConversionType(first.GetType(), second.GetType());

    auto firstConverted = ConvertToType(type, first);
    auto secondConverted = ConvertToType(type, second);

    if (type == ValueType::DOUBLE) {
      PushValue(VMValue{ firstConverted.AsDouble() * secondConverted.AsDouble() }, stack);
    }
    else if (type == ValueType::FLOAT) {
      PushValue(VMValue{ firstConverted.AsFloat() * secondConverted.AsFloat() }, stack);
    }
    else if (type == ValueType::INT) {

      PushValue(VMValue{ firstConverted.AsInt() * secondConverted.AsInt() }, stack);
    }
    else {
      throw std::runtime_error("Invalid type for multiplication: " + TypeToString(type));
    }
  }

  void Div(std::vector<VMValue> &stack) {
    auto second = PopValue(stack);
    auto first = PopValue(stack);
    auto type = GetConversionType(first.GetType(), second.GetType());

    auto firstConverted = ConvertToType(type, first);
    auto secondConverted = ConvertToType(type, second);

    if (type == ValueType::DOUBLE) {
      PushValue(VMValue{ firstConverted.AsDouble() / secondConverted.AsDouble() }, stack);
    }
    else if (type == ValueType::FLOAT) {
      PushValue(VMValue{ firstConverted.AsFloat() / secondConverted.AsFloat() }, stack);
    }
    else if (type == ValueType::INT) {

      PushValue(VMValue{ firstConverted.AsInt() / secondConverted.AsInt() }, stack);
    }
    else {
      throw std::runtime_error("Invalid type for division: " + TypeToString(type));
    }
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

    if (value.AsInt() == 0) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }

  void JumpIfNegative(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);
    if (value.AsInt() < 0) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }

  void JumpIfPositive(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);

    if (value.AsInt() > 0) {
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
    value.SetInt(static_cast<int32_t>(value.AsDouble()));
    PushValue(value, stack);
  }

  void AllocateIntegerArray(std::vector<VMValue> &stack) {
    auto size = PopValue(stack);
    PushValue(MemMgrInstance().AllocateArray(ValueType::INT, size.AsInt()), stack);
  }

  void AllocateObjectArray(std::vector<VMValue> &stack) {
    auto size = PopValue(stack);
    PushValue(MemMgrInstance().AllocateArray(ValueType::MANAGED_POINTER, size.AsInt()), stack);
  }

}