#pragma once
#include "VM/Core/VMValue.h"
template<typename T>
T ToNativeType(VMValue obj) {
  static_assert ( std::is_pointer<T>::value, "This type of conversion is not implemented");
  return (T)obj.AsNativePointer();
}

template<>
std::string ToNativeType<std::string>(VMValue obj);

template<>
int32_t ToNativeType<int32_t>(VMValue obj);

template<>
float ToNativeType<float>(VMValue obj);

template<>
double ToNativeType<double>(VMValue obj);

template<>
bool ToNativeType<bool>(VMValue obj);

template<>
char ToNativeType<char>(VMValue obj);

template<>
VMValue ToNativeType<VMValue>(VMValue obj);

