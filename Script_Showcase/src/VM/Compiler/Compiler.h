#pragma once
#include <string>
#include "VM/Core/VMState.h"

namespace Compiler {
  VMState Compile(const std::string &path);
}