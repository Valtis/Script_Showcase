#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>
enum class ObjectType : uint8_t { UNINITIALIZED, INT, FLOAT, DOUBLE, BOOL, CHAR, NATIVE_POINTER, MANAGED_POINTER };

uint32_t TypeSize(ObjectType type);


class VMValue {
public:

  VMValue() { m_type = ObjectType::UNINITIALIZED; m_value.double_value = 0; }
  VMValue(const int32_t v) { set_int(v); }
  VMValue(const float v) { set_float(v); }
  VMValue(const double v) { set_double(v); }
  VMValue(const bool v) { set_bool(v); }
  VMValue(const char v) { set_char(v); }
  VMValue(void * const v) { set_native_pointer(v); }

  void set_int(const int32_t v) {
    m_type = ObjectType::INT;
    m_value.int_value = v;
  }

  void set_float(const float v) {
    m_type = ObjectType::FLOAT;
    m_value.float_value = v;
  }

  void set_double(const double v) {
    m_type = ObjectType::DOUBLE;
    m_value.double_value = v;
  }

  void set_bool(const bool v) {
    m_type = ObjectType::BOOL;
    m_value.bool_value = v;
  } 
  
  void set_char(const char v) {
    m_type = ObjectType::CHAR;
    m_value.char_value = v;
  }
  
  void set_native_pointer(void * const v) {
    m_type = ObjectType::NATIVE_POINTER;
    m_value.native_pointer_value = v;
  }

  void set_managed_pointer(uint32_t v) {
    m_type = ObjectType::MANAGED_POINTER;
    m_value.managed_pointer_value = v;
  }

  int32_t as_int() const {
    assert_type(ObjectType::INT);
    return m_value.int_value;
  }

  float as_float() const  {
    assert_type(ObjectType::FLOAT);
    return m_value.float_value;
  }

  double as_double() const  {
    assert_type(ObjectType::DOUBLE);
    return m_value.double_value;
  }

  bool as_bool() const  {
    assert_type(ObjectType::BOOL);
    return m_value.bool_value;
  }
  
  char as_char() const {
    assert_type(ObjectType::CHAR);
    return m_value.char_value;
  }

  void *as_native_pointer() const {
    assert_type(ObjectType::NATIVE_POINTER);
    return m_value.native_pointer_value;
  }

  uint32_t as_managed_pointer() const {
    assert_type(ObjectType::MANAGED_POINTER);
    return m_value.managed_pointer_value;
  }

  std::string to_string() const;

private:
  void assert_type(const ObjectType t) const {
    if (m_type != t) {
      throw std::runtime_error("Invalid type");
    }
  }

  ObjectType m_type;

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