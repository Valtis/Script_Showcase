#pragma once
#include "VM/Core/VMValue.h"
#include <functional>
#include <vector>

// struct that containst native binding data
struct NativeBinding {
  std::function<void(std::vector<VMValue> &stack)> function;
  size_t parameterCount;
};
