#pragma once

// Following templates call given function with a tuple that contains the arguments
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

template<typename Function, typename Pointer, typename Tuple, int ...S>
void CallMemberFunctionImpl(Function f, Pointer p, Tuple params, seq<S...>) {
  f(p, std::get<S>(params) ...);
}

template <typename Function, typename Pointer, typename Tuple, typename... Args>
void CallMemberFunction(Function f, Pointer p, Tuple params) {
  CallMemberFunctionImpl(f, p, params, typename gens<sizeof...(Args)>::type());
}
// would be nice if these could somehow merged to the above (or vice versa), as this is basically code duplication. 
// Maybe figure out how to bind the object back to the member function
template<typename Function, typename Tuple, int ...S>
void CallFreeFunctionImpl(Function f, Tuple params, seq<S...>) {
  f(std::get<S>(params) ...);
}

template <typename Function, typename Tuple, typename... Args>
void CallFreeFunction(Function f, Tuple params) {
  CallFreeFunctionImpl(f, params, typename gens<sizeof...(Args)>::type());
}

