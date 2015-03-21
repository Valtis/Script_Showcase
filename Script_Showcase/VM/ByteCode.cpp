#include "ByteCode.h"

const char *GetByteCodeName(ByteCode code) {
  switch (code) {
    case ByteCode::NOP:
      return "NOP";
    case ByteCode::RETURN:
      return "RETURN";
    case ByteCode::PUSH_INTEGER:
      return "PUSH_INTEGER";
    case ByteCode::PUSH_CONSTANT_OBJECT:
      return "PUSH_CONSTANT_OBJECT";
    case ByteCode::INVOKE_NATIVE:
      return "INVOKE_NATIVE";
    case ByteCode::INVOKE_MANAGED:
      return "INVOKE_MANAGED";
    case ByteCode::ADD_INTEGER:
      return "ADD_INTEGER";
    case ByteCode::SUB_INTEGER:
      return "SUB_INTEGER";
    case ByteCode::MUL_INTEGER:
      return "MUL_INTEGER";
    case ByteCode::DIV_INTEGER:
      return "DIV_INTEGER";
    default:
      return "Unknown code";
  }
}
