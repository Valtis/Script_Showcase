#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>
enum class ValueType : uint8_t { UNINITIALIZED, INT, FLOAT, DOUBLE, BOOL, CHAR, NATIVE_POINTER, MANAGED_POINTER };

uint32_t TypeSize(ValueType type);

std::string TypeToString(ValueType t);


// FIXME: Change naming convention to fit the rest of the project
class VMValue {
public:

  VMValue() { m_type = ValueType::UNINITIALIZED; m_value.double_value = 0; } 
  explicit VMValue(const int32_t v) { set_int(v); }
  explicit VMValue(const float v) { set_float(v); }
  explicit VMValue(const double v) { set_double(v); }
  explicit VMValue(const bool v) { set_bool(v); }
  explicit VMValue(const char v) { set_char(v); }
  explicit VMValue(void * const v) { set_native_pointer(v); }
  explicit VMValue(ValueType type) { m_type = type; memset(&m_value, 0, sizeof(m_value)); }

  void set_int(const int32_t v) {
    m_type = ValueType::INT;
    m_value.int_value = v;
  }

  void set_float(const float v) {
    m_type = ValueType::FLOAT;
    m_value.float_value = v;
  }

  void set_double(const double v) {
    m_type = ValueType::DOUBLE;
    m_value.double_value = v;
  }

  void set_bool(const bool v) {
    m_type = ValueType::BOOL;
    m_value.bool_value = v;
  } 
  
  void set_char(const char v) {
    m_type = ValueType::CHAR;
    m_value.char_value = v;
  }
  
  void set_native_pointer(void * const v) {
    m_type = ValueType::NATIVE_POINTER;
    m_value.native_pointer_value = v;
  }

  void set_managed_pointer(uint32_t v) {
    m_type = ValueType::MANAGED_POINTER;
    m_value.managed_pointer_value = v;
  }

  int32_t as_int() const {
    assert_type(ValueType::INT);
    return m_value.int_value;
  }

  float as_float() const  {
    assert_type(ValueType::FLOAT);
    return m_value.float_value;
  }

  double as_double() const  {
    assert_type(ValueType::DOUBLE);
    return m_value.double_value;
  }

  bool as_bool() const  {
    assert_type(ValueType::BOOL);
    return m_value.bool_value;
  }
  
  char as_char() const {
    assert_type(ValueType::CHAR);
    return m_value.char_value;
  }

  void *as_native_pointer() const {
    assert_type(ValueType::NATIVE_POINTER);
    return m_value.native_pointer_value;
  }

  uint32_t as_managed_pointer() const {
    assert_type(ValueType::MANAGED_POINTER);
    return m_value.managed_pointer_value;
  }

  std::string to_string() const;

  ValueType type() {
    return m_type;
  }

  void *value_pointer() { return &m_value; }
    
private:
  void assert_type(const ValueType t) const {
    if (m_type != t) {
      std::string error = "TypeError: Expected " + TypeToString(t) + " but was " + TypeToString(m_type);
      throw std::runtime_error(error);
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
    void *native_pointer_value;
  } m_value;
};