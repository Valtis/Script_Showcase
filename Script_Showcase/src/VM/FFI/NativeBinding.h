#pragma once
#include "VM/Core/VMOperations.h"
#include "VM/Core/VMValue.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/FFI/VariadicFunctionCallTemplates.h"
#include "VM/FFI/FunctionArgumentTupleTemplates.h"
#include "VM/FFI/NativeBindingTypedef.h"

#include <functional>
#include <type_traits>

// binding for void free functions
template <typename ReturnType, typename... Args>
NativeBinding CreateBinding(ReturnType(*ptr)(Args...)) {
  return [=](std::vector<VMValue> &stack) {
    auto parameterTuple = ConstructParameterTuple<Args...>(stack);
    CallFreeFunction<ReturnType, decltype(ptr), decltype(parameterTuple), Args...>(
      stack, ptr, parameterTuple);
  };
}


// binding for void member functions
template <typename Class, typename ReturnType, typename... Args>
NativeBinding CreateBinding(ReturnType (Class::*ptr)(Args...)) {

  return [=](std::vector<VMValue> &stack) {
    auto functionPtr = std::mem_fn(ptr);
    auto parameterTuple = ConstructParameterTuple<Args...>(stack);
    auto classObjectPointer = ToNativeType<Class *>(Op::PopValue(stack));

    CallMemberFunction<ReturnType, decltype(functionPtr), decltype(classObjectPointer), decltype(parameterTuple), Args...>(stack, functionPtr, 
      classObjectPointer, parameterTuple);
  };
}
