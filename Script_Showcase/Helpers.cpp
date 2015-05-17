#include "Helpers.h"
#include <iostream>
#include "VM/Memory/MemoryManager.h"
#include <list>
#include <cstdio>
#include "VM/FFI/NativeBinding.h"

void printer(VMValue value) {
  // really stupid and hacky way of resolving this.
  if (value.GetType() == ValueType::MANAGED_POINTER && MemMgrInstance().IsArray(value) && MemMgrInstance().GetArrayType(value) == ValueType::CHAR) {
    printf("%s", ToNativeType<std::string>(value).c_str());
  }
  else {
    switch (value.GetType()) {
    case ValueType::BOOL: printf("%s", value.AsBool() ? "true" : "false"); break;
    case ValueType::CHAR: printf("%c", value.AsChar()); break;
    case ValueType::DOUBLE: printf("%f", value.AsDouble()); break;
    case ValueType::FLOAT: printf("%f", value.AsFloat()); break;
    case ValueType::INT: printf("%d", value.AsInt()); break;
    default:
      std::cout << value.ToString();
      break;
    }
  }
  std::cout << "\n";
}

float Random() {
  return 4.23f;
}

void AddBindings(std::vector<VMState> &states)  {
  for (auto &state : states) {
    state.AddNativeBinding("printer", CreateBinding(&printer));
    state.AddNativeBinding("memberFunction", CreateBinding(&ExampleClass::ExampleFunction));
    state.AddNativeBinding("getpointers", CreateBinding(&Container::GetPointers));
    state.AddNativeBinding("getintegers", CreateBinding(&Container::GetIntegers));
    state.AddNativeBinding("random", CreateBinding(&Random));
  }
}


ExampleClass::ExampleClass(int value): m_value(value) {

}

void ExampleClass::ExampleFunction(int fromVm) {
  std::cout << "My number is: " << m_value << " and vm value is: " << fromVm << "\n";
}

bool ExampleClass::ExampleFunction2(int fromVm, int fromVm2) {
  std::cout << "My number is: " << m_value << " and vm value is: " << fromVm << "\n";
  return fromVm == fromVm2;
}

Container::Container() {
  for (int i = 0; i < 10; ++i) {
    m_pointers.push_back(new ExampleClass{ i });
    m_integers.push_back(i * 2);
  }
}

Container::~Container() {
  for (auto p : m_pointers) {
    delete p;
  }
}

std::vector<ExampleClass*> Container::GetPointers() {
  return m_pointers;
}

std::list<int> Container::GetIntegers() {
  return m_integers;
}