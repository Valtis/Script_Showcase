#pragma once
#include <cstdint>
enum class ByteCode : uint32_t { NOP, RETURN, PUSH_INTEGER, PUSH_CONSTANT_OBJECT, INVOKE_NATIVE, INVOKE_MANAGED, ADD_INTEGER, SUB_INTEGER, MUL_INTEGER, DIV_INTEGER };


const char *GetByteCodeName(ByteCode code);