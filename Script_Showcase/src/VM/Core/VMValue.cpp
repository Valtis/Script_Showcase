#include "VM/Core/VMValue.h"
uint32_t TypeSize(ValueType type) {
  switch (type) {
  case ValueType::UNINITIALIZED:
    return 0;
  case ValueType::INT:
    return sizeof(int32_t);
  case ValueType::FLOAT:
    return sizeof(float);
  case ValueType::DOUBLE:
    return sizeof(double);
  case ValueType::BOOL:
    return sizeof(bool);
  case ValueType::CHAR:
    return sizeof(char);
  case ValueType::NATIVE_POINTER:
    return sizeof(void *);
  case ValueType::MANAGED_POINTER:
    return sizeof(uint32_t);
  default:
    throw std::runtime_error("Invalid type specified in TypeSize");
  }
}


std::string VMValue::to_string() const {
  std::string str = TypeToString(m_type) + ": ";
   switch (m_type) {
   case ValueType::INT:
     str += std::to_string(m_value.int_value);
     break;
   case ValueType::FLOAT:
     str += std::to_string(m_value.float_value);
     break;
   case ValueType::DOUBLE:
     str += std::to_string(m_value.double_value);
     break;
   case ValueType::BOOL:
     str += std::to_string(m_value.bool_value);
     break;
   case ValueType::CHAR:
     str += std::to_string(m_value.char_value);
     break;
   case ValueType::NATIVE_POINTER:
   {
     std::ostringstream stream;
     stream << "0x"
       << std::setfill('0') << std::setw(sizeof(void *) * 2)
       << std::hex << m_value.native_pointer_value;
     str += stream.str();

   }

   break;
   case ValueType::MANAGED_POINTER:
     str += std::to_string(m_value.managed_pointer_value);
     break;
   case ValueType::UNINITIALIZED:
     str = "Uninitialized value";
     break;
   default:
     str = "Default case reached in VMValue::ToString(). This should not happen";
   }
   return str;
 }


std::string TypeToString(ValueType t) {
  switch (t) {
  case ValueType::INT:
    return "Integer";
  case ValueType::FLOAT:
    return "Float";
  case ValueType::DOUBLE:
    return "Double";
  case ValueType::BOOL:
    return "Boolean";
  case ValueType::CHAR:
    return "Character";
  case ValueType::NATIVE_POINTER:
    return "Native pointer";
    break;
  case ValueType::MANAGED_POINTER:
    return "Managed pointer";
  case ValueType::UNINITIALIZED:
    return "Uninitialized";
  default:
    return "Invalid type tag in TypeToString. Memory corruption likely";
  }
}