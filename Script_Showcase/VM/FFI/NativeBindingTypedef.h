#pragma once
#include "VMValue.h"
#include <functional>
#include <vector>
typedef std::function<void(std::vector<VMValue> &stack)> NativeBinding;