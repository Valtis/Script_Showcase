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

// --- MEMBER FUNCTION VERSIONS
// for void member functions - just calls the function
template<typename ReturnType, typename Function, typename Pointer, typename Tuple, int ...S,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
void CallMemberFunctionImpl(std::vector<VMValue> &stack, Function f, Pointer p, Tuple params, seq<S...>) {
  f(p, std::get<S>(params) ...);
}

// for non-void member functions - pushes the return value into stack
template<typename ReturnType, typename Function, typename Pointer, typename Tuple, int ...S,
  typename std::enable_if<!std::is_void<ReturnType>::value>::type* = nullptr>
void CallMemberFunctionImpl(std::vector<VMValue> &stack, Function f, Pointer p, Tuple params, seq<S...>) {
  stack.push_back(ToManagedType(f(p, std::get<S>(params) ...)));
}

template <typename ReturnType, typename Function, typename Pointer, typename Tuple, typename... Args>
void CallMemberFunction(std::vector<VMValue> &stack, Function f, Pointer p, Tuple params) {
  CallMemberFunctionImpl<ReturnType>(stack, f, p, params, typename gens<sizeof...(Args)>::type());
}


// --- FREE FUNCTION VERSIONS
// version for void free functions - just calls the function
template<typename ReturnType, typename Function, typename Tuple, int ...S,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
void CallFreeFunctionImpl(std::vector<VMValue> &stack, Function f, Tuple params, seq<S...>) {
  f(std::get<S>(params) ...);
}

// non-void version for free functions - pushes the return value into stack
template<typename ReturnType, typename Function, typename Tuple, int ...S,
  typename std::enable_if<!std::is_void<ReturnType>::value>::type* = nullptr>
void CallFreeFunctionImpl(std::vector<VMValue> &stack, Function f, Tuple params, seq<S...>) {
  stack.push_back(ToManagedType(f(std::get<S>(params) ...)));
}

template <typename ReturnType, typename Function, typename Tuple, typename... Args>
void CallFreeFunction(std::vector<VMValue> &stack, Function f, Tuple params) {
  CallFreeFunctionImpl<ReturnType>(stack, f, params, typename gens<sizeof...(Args)>::type());
}

