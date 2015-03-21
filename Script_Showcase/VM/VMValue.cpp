#include "VMValue.h"
uint32_t TypeSize(ObjectType type) {
  switch (type) {
  case ObjectType::UNINITIALIZED:
    return 0;
  case ObjectType::INT:
    return sizeof(int32_t);
  case ObjectType::FLOAT:
    return sizeof(float);
  case ObjectType::DOUBLE:
    return sizeof(double);
  case ObjectType::BOOL:
    return sizeof(bool);
  case ObjectType::CHAR:
    return sizeof(char);
  case ObjectType::NATIVE_POINTER:
    return sizeof(void *);
  case ObjectType::MANAGED_POINTER:
  default:
    return 0;
  }
}


std::string VMValue::to_string() const {
   std::string str;
   switch (m_type) {
   case ObjectType::INT:
     str = "Integer: " + std::to_string(m_value.int_value);
     break;
   case ObjectType::FLOAT:
     str = "Float: " + std::to_string(m_value.float_value);
     break;
   case ObjectType::DOUBLE:
     str = "Double: " + std::to_string(m_value.double_value);
     break;
   case ObjectType::BOOL:
     str = "Boolean: " + std::to_string(m_value.bool_value);
     break;
   case ObjectType::CHAR:
     str = "Char: " + std::to_string(m_value.char_value);
     break;
   case ObjectType::NATIVE_POINTER:
   {
     std::ostringstream stream;
     stream << "0x"
       << std::setfill('0') << std::setw(sizeof(void *) * 2)
       << std::hex << m_value.native_pointer_value;
     str = "Native pointer: " + stream.str();

   }

   break;
   case ObjectType::MANAGED_POINTER:
     str = "Managed pointer: " + std::to_string(m_value.managed_pointer_value);
     break;

   default:
     str = "Unknown type";
   }
   return str;
 }