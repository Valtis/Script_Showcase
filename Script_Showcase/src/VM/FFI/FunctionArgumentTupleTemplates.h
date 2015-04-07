#pragma once
#include "VM/Core/VMValue.h"
#include <vector>
#include <tuple>

// template that creates and fills tuple with type of ParameterTypes parameter pack (eg. ConstructParameterTuple<int, int, float> returns tuple<int, int, float>).
// Parameters are popped from stack and stored in the tuple


// base case for when whole tuple has been handled - do nothing
template <size_t position, typename Tuple>
void SetTupleValue(Tuple &t, std::vector<VMValue> &stack) {

}


// main recursive template. Writes parameter n value into tuple index n
template<size_t position, typename Tuple, typename ParameterType, typename ...RemainingParameterTypes>
void SetTupleValue(Tuple &t, std::vector<VMValue> &stack) {

  SetTupleValue<position + 1, decltype(t), RemainingParameterTypes...>(t, stack);

  // topmost value in stack is the last parameter , so pop only after everything else is handled
  std::get<position>(t) = ToNativeType<ParameterType>(Op::PopValue(stack)); }

template<typename ...ParameterTypes>
std::tuple<ParameterTypes...> ConstructParameterTuple(std::vector<VMValue> &stack) {
  std::tuple<ParameterTypes...> t;
  SetTupleValue<0, decltype(t), ParameterTypes...>(t, stack);
  return t;
}
