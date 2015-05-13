#pragma once
#include "VM/Core/VMValue.h"

/*
  Conversions from VMValue into native types and native types into managed types. 
  Invalid managed-to-native conversion will throw if there is a type mismatch (for example, trying to convert double into integer)

  These are primarily used by the native bindings when parameters need to be passed between VM and native code.
*/

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



template<typename T>
VMValue ToManagedType(const T &obj) {
  return VMValue{ obj };
}

template<typename T>
ValueType GetType() {
  static_assert (std::is_pointer<T>::value, "This type of conversion is not implemented");
  return ValueType::NATIVE_POINTER;
}

template<>
ValueType GetType<int32_t>();

template<>
ValueType GetType<float>();

template<>
ValueType GetType<double>();

template<>
ValueType GetType<bool>();

template<>
ValueType GetType<char>();

// specialization for handling c++ containers
template<template<typename, typename...> class Container, typename Type, typename... Args>
VMValue ToManagedType(const Container<Type, Args...> &container) {
  auto type = GetType<Type>();
  auto &memMgr = MemMgrInstance();
  auto array = memMgr.AllocateArray(type, container.size());

  uint32_t index = 0;
  for (const auto &c : container) {
    memMgr.WriteToArrayIndex(array, &c, index++, 1);
  }

  return array;
}

