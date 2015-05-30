#pragma once
#include "VM/Exceptions/Exceptions.h"
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
enum class ValueType : uint8_t { UNINITIALIZED, INT, FLOAT, DOUBLE, BOOL, CHAR, NATIVE_POINTER, MANAGED_POINTER, FUNCTION };

uint32_t TypeSize(ValueType type);

std::string TypeToString(ValueType t);

/*
  Represents a value. Contains type field and a union with all the possible types. Performs type checks when attempting to access the values.
  AsX-functions throw if there is type mismatch.
*/

class VMValue {
public:

  VMValue() { m_type = ValueType::UNINITIALIZED; m_value.double_value = 0; }
  explicit VMValue(const int32_t v) { SetInt(v); }
  explicit VMValue(const float v) { SetFloat(v); }
  explicit VMValue(const double v) { SetDouble(v); }
  explicit VMValue(const bool v) { SetBool(v); }
  explicit VMValue(const char v) { SetChar(v); }
  explicit VMValue(void * const v) { SetNativePointer(v); }
  explicit VMValue(ValueType type) { m_type = type; memset(&m_value, 0, sizeof(m_value)); }

  VMValue operator+(const VMValue &rhs) const;
  VMValue operator-(const VMValue &rhs) const;
  VMValue operator*(const VMValue &rhs) const;
  VMValue operator/(const VMValue &rhs) const;
  VMValue operator%(const VMValue &rhs) const;

  bool operator>=(const VMValue &rhs) const;
  bool operator>(const VMValue &rhs) const;
  bool operator==(const VMValue &rhs) const;
  bool operator<=(const VMValue &rhs) const;
  bool operator<(const VMValue &rhs) const;

  void SetInt(const int32_t v) {
    m_type = ValueType::INT;
    m_value.int_value = v;
  }

  void SetFloat(const float v) {
    m_type = ValueType::FLOAT;
    m_value.float_value = v;
  }

  void SetDouble(const double v) {
    m_type = ValueType::DOUBLE;
    m_value.double_value = v;
  }

  void SetBool(const bool v) {
    m_type = ValueType::BOOL;
    m_value.bool_value = v;
  } 
  
  void SetChar(const char v) {
    m_type = ValueType::CHAR;
    m_value.char_value = v;
  }
  
  void SetNativePointer(void * const v) {
    m_type = ValueType::NATIVE_POINTER;
    m_value.native_pointer_value = v;
  }

  void SetManagedPointer(uint32_t v) {
    m_type = ValueType::MANAGED_POINTER;
    m_value.managed_pointer_value = v;
  }

  void SetFunction(uint32_t v) {
    m_type = ValueType::FUNCTION;
    m_value.function_value = v;
  }

  int32_t AsInt() const {
    AssertType(ValueType::INT);
    return m_value.int_value;
  }

  float AsFloat() const  {
    AssertType(ValueType::FLOAT);
    return m_value.float_value;
  }

  double AsDouble() const  {
    AssertType(ValueType::DOUBLE);
    return m_value.double_value;
  }

  bool AsBool() const  {
    AssertType(ValueType::BOOL);
    return m_value.bool_value;
  }
  
  char AsChar() const {
    AssertType(ValueType::CHAR);
    return m_value.char_value;
  }

  void *AsNativePointer() const {
    AssertType(ValueType::NATIVE_POINTER);
    return m_value.native_pointer_value;
  }

  uint32_t AsManagedPointer() const {
    AssertType(ValueType::MANAGED_POINTER);
    return m_value.managed_pointer_value;
  } 

  uint32_t AsFunction() const {
    AssertType(ValueType::FUNCTION);
    return m_value.function_value;
  }


  std::string ToString() const;

  ValueType GetType() const {
    return m_type;
  }

  void *ValuePointer() { return &m_value; }
    
private:
  void AssertType(const ValueType t) const {
    if (m_type != t) {
      std::string error = "TypeError: Expected " + TypeToString(t) + " but was " + TypeToString(m_type);
      throw TypeError(error);
    }
   }

  ValueType m_type;

  union {
    int32_t int_value;
    float float_value;
    double double_value;
    bool bool_value;
    char char_value;
    uint32_t managed_pointer_value;
    uint32_t function_value;
    void *native_pointer_value;
  } m_value;
};