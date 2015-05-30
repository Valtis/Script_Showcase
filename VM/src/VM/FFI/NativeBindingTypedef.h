#pragma once
#include "VM/Core/VMValue.h"
#include <functional>
#include <vector>
// typedef in separate file due to performance. Large chunks of cpp files would otherwise have to be recompiled when NativeBinding.h changes
typedef std::function<void(std::vector<VMValue> &stack)> NativeBinding;