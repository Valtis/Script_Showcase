#pragma once
#include "VM/FFI/ConversionFunctions.h"
#include "VM/Core/VMValue.h"
// Following templates call given function where parameters are stored in a tuple
// Thanks to user 'Johannes Schaub - litb' from Stack Overflow. Slightly modified to fit the existing code and naming scheme
// http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
template<int ...>
struct seq { };

template<int N, int ...S>
struct gens : gens < N - 1, N - 1, S... > { };

template<int ...S>
struct gens < 0, S... > {
  typedef seq<S...> type;
};

// version for void free functions - just calls the function
template<typename ReturnType, typename Function, typename Tuple, int ...S,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
void CallFunctionImpl(std::vector<VMValue> &stack, Function f, Tuple params, seq<S...>) {
  f(std::get<S>(params) ...);
}
 
// non-void version for free functions - pushes the return value into stack
template<typename ReturnType, typename Function, typename Tuple, int ...S,
  typename std::enable_if<!std::is_void<ReturnType>::value>::type* = nullptr>
void CallFunctionImpl(std::vector<VMValue> &stack, Function f, Tuple params, seq<S...>) {
  stack.push_back(ToManagedType(f(std::get<S>(params) ...)));
}

template <typename ReturnType, typename Function, typename Tuple, typename... Args>
void CallFunction(std::vector<VMValue> &stack, Function f, Tuple params) {
  CallFunctionImpl<ReturnType>(stack, f, params, typename gens<sizeof...(Args)>::type());
}

