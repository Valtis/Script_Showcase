#pragma once
#include "VM/Core/VMValue.h"
#include "VM/FFI/VariadicFunctionCallTemplates.h"
#include "VM/FFI/FunctionArgumentTupleTemplates.h"
#include "VM/FFI/NativeBindingStruct.h"

#include <functional>
/*
  Wraps the given function pointer into lambda function that handles the argument passing and invocation.
  
  First pops parameters from VM stack and stores them in a tuple. Tuple values are then passed as arguments for the function pointer.
  If native function has a return value, the value is pushed into VM stack.

*/

// binding for free functions
template <typename ReturnType, typename... Args>
NativeBinding CreateBinding(ReturnType(*ptr)(Args...)) {
  NativeBinding binding;
  binding.parameterCount = sizeof...(Args);

  binding.function = [=](std::vector<VMValue> &stack) {
    auto parameterTuple = ConstructParameterTuple<Args...>(stack);
    CallFunction<ReturnType, decltype(ptr), decltype(parameterTuple), Args...>(
      stack, ptr, parameterTuple);
  };

  return binding;
}

// binding for member functions
template <typename Class, typename ReturnType, typename... Args>
NativeBinding CreateBinding(ReturnType (Class::*ptr)(Args...)) {
  NativeBinding binding;
  binding.parameterCount = sizeof...(Args);

  binding.function = [=](std::vector<VMValue> &stack) {
    auto functionPtr = std::mem_fn(ptr);
    auto parameterTuple = ConstructParameterTuple<Class *, Args...>(stack);
     
    CallFunction<ReturnType, decltype(functionPtr), decltype(parameterTuple), Class *, Args...>(
      stack, functionPtr, parameterTuple);
  };

  return binding;
}
