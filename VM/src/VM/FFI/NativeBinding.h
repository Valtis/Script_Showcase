#pragma once
#include "VM/Core/VMOperations.h"
#include "VM/Core/VMValue.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/FFI/VariadicFunctionCallTemplates.h"
#include "VM/FFI/FunctionArgumentTupleTemplates.h"
#include "VM/FFI/NativeBindingTypedef.h"

#include <functional>
/*
  Wraps the given function pointer into lambda function that handles the argument passing and invokation.
  
  First pops parameters from VM stack and stores them in a tuple. Tuple values are then passed as arguments for the function pointer.
  If native function has a return value, the value is pushed into VM stack.

*/

// binding for free functions
template <typename ReturnType, typename... Args>
NativeBinding CreateBinding(ReturnType(*ptr)(Args...)) {
  return [=](std::vector<VMValue> &stack) {
    auto parameterTuple = ConstructParameterTuple<Args...>(stack);
    CallFreeFunction<ReturnType, decltype(ptr), decltype(parameterTuple), Args...>(
      stack, ptr, parameterTuple);
  };
}

// binding for member functions
template <typename Class, typename ReturnType, typename... Args>
NativeBinding CreateBinding(ReturnType (Class::*ptr)(Args...)) {

  return [=](std::vector<VMValue> &stack) {
    auto functionPtr = std::mem_fn(ptr);
    auto parameterTuple = ConstructParameterTuple<Class *, Args...>(stack);

    auto classObjectPointer = ToNativeType<Class *>(Op::PopValue(stack));
 
    CallFreeFunction<ReturnType, decltype(functionPtr), decltype(parameterTuple), Class *, Args...>(
      stack, functionPtr, parameterTuple);
  };
}
