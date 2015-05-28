#include "VM/Core/VMOperations.h"
#include "VM/Core/VMValue.h"
#include "VM/Core/VMFrame.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMFunction.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/Memory/MemoryManager.h"
#include <functional>

namespace Op {

  // TODO: Cleanup
  ValueType GetConversionType(ValueType first, ValueType second) {

    if (first == second) {
      return first;
    }

    if (first == ValueType::DOUBLE || second == ValueType::DOUBLE) {
      return ValueType::DOUBLE;
    }

    if (first == ValueType::FLOAT || second == ValueType::FLOAT) {
      return ValueType::FLOAT;
    }

    throw std::runtime_error("No conversion exists between " + TypeToString(second) + " and " + TypeToString(first));
  }

  VMValue ConvertToType(ValueType type, VMValue value) {

    // if the value already has the type, no conversion is required.
    if (value.GetType() == type) {
      return value;
    }

    switch (type) {
    case ValueType::DOUBLE:

      if (value.GetType() == ValueType::FLOAT) {
        return VMValue{ (double)value.AsFloat() };
      }
      else if (value.GetType() == ValueType::INT) {
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
        return VMValue{ (int32_t )value.AsFloat() };
      }
      throw std::runtime_error("TypeError: Could not convert " + TypeToString(value.GetType()) + " to " + TypeToString(type));
    default:
      return value;
    }
    return value;
  }


  void GetTwoParametersWithTypeConversion(std::vector<VMValue>& stack, ValueType& type, VMValue& firstConverted, VMValue& secondConverted) {
    auto second = PopValue(stack);
    auto first = PopValue(stack);
    type = GetConversionType(first.GetType(), second.GetType());

    firstConverted = ConvertToType(type, first);
    secondConverted = ConvertToType(type, second);
  }


  void PushValue(const VMValue &value, std::vector<VMValue> &stack) {
    if (stack.size() == STACK_SIZE) {
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

  void PushFunction(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    VMValue value;
    value.SetFunction(static_cast<uint32_t>(frames.back().GetNextInstruction()));
    PushValue(value, stack);
  }

  void PushFloat(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto val = static_cast<uint32_t>(frames.back().GetNextInstruction());
    PushValue(VMValue{ *reinterpret_cast<float*>(&val) }, stack);
  }

  void PushDouble(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    // here be dragons
    auto highbits = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto lowbits = static_cast<uint32_t>(frames.back().GetNextInstruction());

    uint64_t val = 0;

    auto ptr = reinterpret_cast<char *>(&val);
    auto highPtr = reinterpret_cast<char *>(&highbits);
    auto lowPtr = reinterpret_cast<char *>(&lowbits);

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

  void PushBoolean(std::vector<VMValue>& stack, std::vector<VMFrame>& frames) {
    auto boolean = static_cast<uint32_t>(frames.back().GetNextInstruction());
    PushValue(VMValue{ boolean != 0}, stack);
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
    auto index = PopValue(stack);
    auto array = PopValue(stack);
    auto type = MemMgrInstance().GetArrayType(array);

    VMValue value{ type };
    MemMgrInstance().ReadFromArrayIndex(array, value.ValuePointer(), index.AsInt(), 1);
    PushValue(value, stack);
  }

  void StoreArrayIndex(std::vector<VMValue> &stack) {
    auto value = PopValue(stack);
    auto index = PopValue(stack); 
    auto array = PopValue(stack);
    auto type = MemMgrInstance().GetArrayType(array);
    if (type != value.GetType()) {
      throw std::runtime_error("Array type and value type mismatch: Array is of type " + TypeToString(type) + " and value is of type " +
        TypeToString(value.GetType()));
    }

    MemMgrInstance().WriteToArrayIndex(array, value.ValuePointer(), index.AsInt(), 1);
  }

  void ArrayLength(std::vector<VMValue>& stack) {
    auto array = PopValue(stack);
    PushValue(VMValue{ static_cast<int32_t>(MemMgrInstance().GetArrayLength(array)) }, stack);

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
    if (second == 0) {
      throw std::runtime_error("Division by zero");
    }

    auto first = PopValue(stack).AsInt();
    PushValue(VMValue{ first / second }, stack);
  }

  template <template<typename> class Operand>
  void AritmeticOperationWithTypeConversion(std::vector<VMValue> &stack) {
    ValueType type; 
    VMValue firstConverted; 
    VMValue secondConverted; 
    GetTwoParametersWithTypeConversion(stack, type, firstConverted, secondConverted);
    // some operands may return boolean instead of VMValue, so need to wrap this in constructor call
    VMValue result = VMValue{ Operand<VMValue>()(firstConverted, secondConverted) }; 

    PushValue(result, stack);;
  }



  void Add(std::vector<VMValue> &stack) {
    AritmeticOperationWithTypeConversion<std::plus>(stack);
  }

  void Sub(std::vector<VMValue> &stack) {
    AritmeticOperationWithTypeConversion<std::minus>(stack);
  } 
  
  void Mul(std::vector<VMValue> &stack) {
    AritmeticOperationWithTypeConversion<std::multiplies>(stack);
  }

  void Div(std::vector<VMValue> &stack) {
    AritmeticOperationWithTypeConversion<std::divides>(stack);
  }

  void Mod(std::vector<VMValue>& stack) {
    AritmeticOperationWithTypeConversion<std::modulus>(stack);
  }
  
  void Not(std::vector<VMValue>& stack) {
    auto first = PopValue(stack);
    PushValue(VMValue{ !first.AsBool() }, stack);
  }

  void IsGreater(std::vector<VMValue> &stack) {
    AritmeticOperationWithTypeConversion<std::greater>(stack);
  }

  void IsGreaterOrEq(std::vector<VMValue> &stack) {
    AritmeticOperationWithTypeConversion<std::greater_equal>(stack);
  }

  void IsEq(std::vector<VMValue> &stack) {
    AritmeticOperationWithTypeConversion<std::equal_to>(stack);
  }  
  
  void IsLessOrEq(std::vector<VMValue> &stack) {
    AritmeticOperationWithTypeConversion<std::less_equal>(stack);
  } 
  
  void IsLess(std::vector<VMValue> &stack) {
    AritmeticOperationWithTypeConversion<std::less>(stack);
  }

  void InvokeNative(const VMState &state, std::vector<VMValue> &stack) {
    auto ptrToStr = PopValue(stack);
    auto binding = state.GetNativeBinding(ToNativeType<std::string>(ptrToStr));
    binding(stack);
  }

  void InvokeManaged(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto function = state.GetFunction(index);

    if (frames.size() == FRAME_SIZE) {
      throw std::runtime_error("Maximum number of frames reached - stack overflow");
    }
    frames.push_back(function);
  }

  void InvokeManagedIndirect(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    if (frames.size() == FRAME_SIZE) {
      throw std::runtime_error("Maximum number of frames reached - stack overflow");
    }
    
    auto index = PopValue(stack).AsFunction();
    auto function = state.GetFunction(index);

    auto paramCount = PopValue(stack).AsInt();
    if (function->GetArgumentCount() != paramCount) {
      throw std::runtime_error("Invalid argument count for function " + function->GetName() + ". " 
        + std::to_string(paramCount) + " were provided when " + std::to_string(function->GetArgumentCount()) + " were expected.");
    }

    frames.push_back(function);
  }

  bool Return(std::vector<VMFrame> &frames) {
    frames.pop_back();
    if (frames.empty())  {
      return false;
    }
    return true;

  }

  void Jump(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = static_cast<uint32_t>(frames.back().GetNextInstruction());
    frames.back().SetNextInstruction(jumpDestination);
  }


  void JumpIfTrue(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);

    if (value.AsBool() == true) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }


  void JumpIfFalse(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);

    if (value.AsBool() == false) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }

  void StoreLocal(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = static_cast<uint32_t>(frames.back().GetNextInstruction());
    auto value = PopValue(stack);

    frames.back().SetLocalVariable(index, value);
  }

  void LoadLocal(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = static_cast<uint32_t>(frames.back().GetNextInstruction());
    PushValue(frames.back().GetLocalVariable(index), stack);
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