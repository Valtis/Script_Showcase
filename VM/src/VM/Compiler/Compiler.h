#pragma once
#include <string>
#include "VM/Core/VMState.h"
/*
  Given path to script file, this class produces VMState with bytecode, functions and possible static values.
*/
namespace Compiler {
  VMState Compile(const std::string &path);
}