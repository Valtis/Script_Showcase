#include "VM/Core/ByteCode.h"


const char *GetByteCodeName(ByteCode code) {
  switch (code) {
    case ByteCode::NOP: return "NOP";
    case ByteCode::RETURN: return "RETURN";
    case ByteCode::PUSH_INTEGER: return "PUSH_INTEGER";
    case ByteCode::PUSH_FLOAT: return "PUSH_FLOAT";
    case ByteCode::PUSH_DOUBLE: return "PUSH_DOUBLE";
    case ByteCode::PUSH_BOOLEAN: return "PUSH_BOOLEAN";
    case ByteCode::POP: return "POP";
    case ByteCode::STORE_STATIC_OBJECT:  return "STORE_STATIC_OBJECT";
    case ByteCode::LOAD_STATIC_OBJECT: return "LOAD_STATIC_OBJECT";
    case ByteCode::STORE_LOCAL: return "STORE_LOCAL";
    case ByteCode::LOAD_LOCAL:  return "LOAD_LOCAL";
    case ByteCode::STORE_ARRAY_INDEX: return "STORE_ARRAY_INDEX";
    case ByteCode::LOAD_ARRAY_INDEX: return "LOAD_ARRAY_INDEX";
    case ByteCode::ARRAY_LENGTH: return "ARRAY_LENGTH";
    case ByteCode::INVOKE_NATIVE: return "INVOKE_NATIVE";
    case ByteCode::INVOKE_MANAGED: return "INVOKE_MANAGED";    
    case ByteCode::ADD: return "ADD";
    case ByteCode::SUB: return "SUB";
    case ByteCode::MUL:return "MUL";
    case ByteCode::DIV: return "DIV";
    case ByteCode::MOD: return "DIV";
    case ByteCode::ADD_INTEGER: return "ADD_INTEGER";
    case ByteCode::SUB_INTEGER: return "SUB_INTEGER";
    case ByteCode::MUL_INTEGER: return "MUL_INTEGER";
    case ByteCode::DIV_INTEGER: return "DIV_INTEGER";
    case ByteCode::IS_GREATER: return "IS_GREATER";
    case ByteCode::IS_GREATER_OR_EQ: return "IS_GREATER_OR_EQ";
    case ByteCode::IS_EQ: return "IS_EQ";
    case ByteCode::IS_LESS_OR_EQ: return "IS_LESS_OR_EQ";
    case ByteCode::IS_LESS: return "IS_LESS";
    case ByteCode::JUMP: return "JUMP_IF_TRUE";
    case ByteCode::JUMP_IF_TRUE: return "JUMP_IF_TRUE";
    case ByteCode::JUMP_IF_FALSE: return "JUMP_IF_FALSE";
    case ByteCode::JUMP_IF_ZERO:  return "JUMP_IF_ZERO";
    case ByteCode::JUMP_IF_NEGATIVE: return "JUMP_IF_NEGATIVE";
    case ByteCode::JUMP_IF_POSITIVE: return "Jump_IF_POSITIVE";
    case ByteCode::DOUBLE_TO_INTEGER: return "DOUBLE_TO_INTEGER";
    case ByteCode::ALLOCATE_INTEGER_ARRAY: return "ALLOCATE_INTEGER_ARRAY";
    case ByteCode::ALLOCATE_OBJECT_ARRAY: return "ALLOCATE_OBJECT_ARRAY";
    default:
      return "Unknown code";
  }
}
