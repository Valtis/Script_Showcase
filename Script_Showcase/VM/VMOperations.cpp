#include "VMOperations.h"
#include "VMValue.h"
#include "VMFrame.h"
#include "VMState.h"
#include "VMFunction.h"
#include "FFI/ConversionFunctions.h"
#include "FFI/NativeBinding.h"

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
    PushValue((int32_t)frames.back().GetNextInstruction(), stack);
  }


  void PushConstantObject(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    PushValue(state.GetPermanentStorageObject((uint32_t)frames.back().GetNextInstruction()), stack);
  }

  void AddInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(first + second, stack);
  }

  void SubInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(first - second, stack);
  }  

  void MulInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(first * second, stack);
  }  
  
  void DivInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(first / second, stack);
  }
  
  void InvokeNative(const VMState &state, std::vector<VMValue> &stack) {
    auto ptrToStr = PopValue(stack);
    auto binding = state.GetNativeBinding(ToNativeType<std::string>(ptrToStr));
    binding(stack);
  }

 void InvokeManaged(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto ptrToName = PopValue(stack);
    auto name = ToNativeType<std::string>(ptrToName);
    auto function = state.GetFunction(name);
    if (function == nullptr) {
      throw std::runtime_error("Could not invoke function " + name + ": No such function exists");
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
  

}