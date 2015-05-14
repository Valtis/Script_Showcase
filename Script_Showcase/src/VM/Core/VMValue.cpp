#include "VM/Core/VMValue.h"
#include <functional>
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
  case ValueType::FUNCTION:
    return sizeof(uint32_t);
  default:
    throw std::runtime_error("Invalid type specified in TypeSize");
  }
}


void AssertSameType(const VMValue &lhs, const VMValue& rhs) {
  if (lhs.GetType() != rhs.GetType()) {
    throw std::runtime_error("TypeError: Expected types to be same but were " + TypeToString(lhs.GetType()) + " and " + TypeToString(rhs.GetType()));
  }
}

template <template<typename> class Operand>
VMValue CommonNumberOperation(const VMValue &lhs, const VMValue &rhs) {
  AssertSameType(lhs, rhs);

  switch (lhs.GetType()) {
  case ValueType::INT:
    return VMValue{ Operand<int>()(lhs.AsInt(), rhs.AsInt()) };
  case ValueType::FLOAT:
    return VMValue{ Operand<float>()(lhs.AsFloat(), rhs.AsFloat()) };
  case ValueType::DOUBLE:
    return VMValue{ Operand<double>()(lhs.AsDouble(), rhs.AsDouble()) };
  default:
    // do nothing
    break;
  }  
  return VMValue{};
}
// const char param in order to eliminate unnecessary allocation if operation succeeds
void CheckOperationSuccess(const ValueType originaltType, const VMValue &value, const char *operationName) {
  if (value.GetType() == ValueType::UNINITIALIZED) {
    throw std::runtime_error(std::string(operationName) + " is not implemented for " + TypeToString(originaltType));
  }  
}

VMValue VMValue::operator+(const VMValue& rhs)  const {
  auto value = CommonNumberOperation<std::plus>(*this, rhs);
  CheckOperationSuccess(m_type, value, "Addition");
  return value;
}

VMValue VMValue::operator-(const VMValue& rhs) const {
  auto value = CommonNumberOperation<std::minus>(*this, rhs); 
  CheckOperationSuccess(m_type, value, "Substraction");
  return value;
}


VMValue VMValue::operator*(const VMValue& rhs) const {
  auto value = CommonNumberOperation<std::multiplies>(*this, rhs);
  CheckOperationSuccess(m_type, value, "Multiplication");
  return value;
}

VMValue VMValue::operator/(const VMValue& rhs) const {
  if (m_type == ValueType::INT && rhs.m_value.int_value == 0) {
    throw std::runtime_error("Division by zero");
  }

  auto value = CommonNumberOperation<std::divides>(*this, rhs);
  CheckOperationSuccess(m_type, value, "Division");
  return value;
}


VMValue VMValue::operator%(const VMValue &rhs) const {
  AssertSameType(*this, rhs);
  if (m_type != ValueType::INT) {
    throw std::runtime_error("Modulo is only implemented for integer types");
  }

  if (rhs.m_value.int_value == 0) {
    throw std::runtime_error("Division by zero");
  }

  return VMValue{ m_value.int_value % rhs.m_value.int_value };
}

bool VMValue::operator>=(const VMValue &rhs) const {
  auto value = CommonNumberOperation<std::greater_equal>(*this, rhs);
  CheckOperationSuccess(m_type, value, "Greater or equal than");
  return value.m_value.bool_value;
}

bool VMValue::operator>(const VMValue &rhs) const {
  auto value = CommonNumberOperation<std::greater>(*this, rhs);
  CheckOperationSuccess(m_type, value, "Greater than");
  return value.m_value.bool_value;
}

bool VMValue::operator==(const VMValue &rhs) const {
  AssertSameType(*this, rhs);
  return !std::memcmp(&m_value, &rhs.m_value, sizeof(m_value));
}

bool VMValue::operator<=(const VMValue &rhs) const {
  auto value = CommonNumberOperation<std::less_equal>(*this, rhs);
  CheckOperationSuccess(m_type, value, "Less or equal than");
  return value.m_value.bool_value;
}
bool VMValue::operator<(const VMValue &rhs) const {
  auto value = CommonNumberOperation<std::less>(*this, rhs);
  CheckOperationSuccess(m_type, value, "Less than");
  return value.m_value.bool_value;
}

std::string VMValue::ToString() const {
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
   case ValueType::FUNCTION:
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
  case ValueType::FUNCTION:
      return "Function";
  case ValueType::UNINITIALIZED:
    return "Uninitialized";
  default:
    return "Invalid type tag in TypeToString. Memory corruption likely";
  }
}