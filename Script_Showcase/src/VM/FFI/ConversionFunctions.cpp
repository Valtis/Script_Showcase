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
   return obj.as_int();
}

template<>
float ToNativeType<float>(VMValue obj) {
  return obj.as_float();
}

template<>
double ToNativeType<double>(VMValue obj) {
  return obj.as_double();
}

template<>
bool ToNativeType<bool>(VMValue obj) {
  return obj.as_bool();
}

template<>
char ToNativeType<char>(VMValue obj) {
  return obj.as_char();
}

template<>
VMValue ToNativeType<VMValue>(VMValue obj) {
  return obj;
}
