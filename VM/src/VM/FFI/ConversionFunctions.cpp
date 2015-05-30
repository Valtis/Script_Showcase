#include "VM/FFI/ConversionFunctions.h"
#include "VM/Memory/MemoryManager.h"
#include <string>
template<>
std::string ToNativeType<std::string>(VMValue obj) {
   std::string str;
   auto length = MemMgrInstance().GetArrayLength(obj);
   str.resize(length);
   MemMgrInstance().ReadFromArrayIndex(obj, &str[0], 0 , length);
   return str;
}

template<> 
int32_t ToNativeType<int32_t>(VMValue obj) {
   return obj.AsInt();
}

template<>
float ToNativeType<float>(VMValue obj) {
  return obj.AsFloat();
}

template<>
double ToNativeType<double>(VMValue obj) {
  return obj.AsDouble();
}

template<>
bool ToNativeType<bool>(VMValue obj) {
  return obj.AsBool();
}

template<>
char ToNativeType<char>(VMValue obj) {
  return obj.AsChar();
}

template<>
VMValue ToNativeType<VMValue>(VMValue obj) {
  return obj;
}


template<>
ValueType GetType<int32_t>() {
  return ValueType::INT;
}


template<>
ValueType GetType<float>() {
  return ValueType::FLOAT;
}



template<>
ValueType GetType<double>() {
  return ValueType::DOUBLE;
}

template<>
ValueType GetType<bool>() {
  return ValueType::BOOL;
}

template<>
ValueType GetType<char>() {
  return ValueType::CHAR;
}

