#pragma once
#include <cstdint>
/*
  Bytecodes
*/

// size of the enum should not be changed, as many instructions have parameters encoded into the instruction stream
// and these make assumptions about the size of the ByteCode enum.
enum class ByteCode : uint32_t { 
  NOP,
  RETURN,
  PUSH_INTEGER,
  PUSH_FLOAT,
  PUSH_DOUBLE,
  PUSH_BOOLEAN,
  PUSH_FUNCTION,
  POP,
  STORE_STATIC_OBJECT,
  LOAD_STATIC_OBJECT,
  STORE_LOCAL,
  LOAD_LOCAL,
  STORE_ARRAY_INDEX,
  LOAD_ARRAY_INDEX,
  ARRAY_LENGTH,
  INVOKE_NATIVE,
  INVOKE_MANAGED, 
  INVOKE_MANAGED_INDIRECT,
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  ADD_INTEGER, 
  SUB_INTEGER, 
  MUL_INTEGER, 
  DIV_INTEGER,
  IS_GREATER,
  IS_GREATER_OR_EQ,
  IS_EQ,
  IS_LESS_OR_EQ,
  IS_LESS,
  JUMP,
  JUMP_IF_TRUE,
  JUMP_IF_FALSE,
  JUMP_IF_ZERO,
  JUMP_IF_NEGATIVE,
  JUMP_IF_POSITIVE, 
  DOUBLE_TO_INTEGER,
  ALLOCATE_INTEGER_ARRAY,
  ALLOCATE_OBJECT_ARRAY
};


const char *GetByteCodeName(ByteCode code);