#include <iostream>
#include "Utility/LoggerManager.h"
#include "VM/Core/VM.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMValue.h"
#include "VM/FFI/NativeBinding.h"
#include "VM/Compiler/Compiler.h"
#include "VM/Memory/MemoryManager.h"
#include <vector>
#include <list>
#include <cstdio>

#include <windows.h>
void printer(VMValue value) {
  // really stupid and hacky way of resolving this.
  if (value.GetType() == ValueType::MANAGED_POINTER && MemMgrInstance().IsArray(value) && MemMgrInstance().GetArrayType(value) == ValueType::CHAR) {
   printf("%s", ToNativeType<std::string>(value).c_str());
  } else {
    switch (value.GetType()) {
    case ValueType::BOOL: printf("%s", value.AsBool() ? "true" : "false"); break;
    case ValueType::CHAR: printf("%c", value.AsChar()); break;
    case ValueType::DOUBLE: printf("%f",value.AsDouble()); break;
    case ValueType::FLOAT: printf("%f", value.AsFloat()); break;
    case ValueType::INT: printf("%d", value.AsInt()); break;
    case ValueType::UNINITIALIZED:
    case ValueType::MANAGED_POINTER:
    case ValueType::NATIVE_POINTER:
    case ValueType::FUNCTION:
      std::cout << value.ToString();
      break;
    }
  }
  std::cout << "\n";
}

class ExampleClass
{
public:
  ExampleClass(int value) : m_value(value) {

  }

  void ExampleFunction(int fromVm) {
    std::cout << "My number is: " << m_value << " and vm value is: " << fromVm << "\n";
  }

  bool ExampleFunction2(int fromVm, int fromVm2) {
    std::cout << "My number is: " << m_value << " and vm value is: " << fromVm << "\n";
    return fromVm == fromVm2;
  }
private:
  int m_value;
};

class Container {
public:
  Container() {
    for (int i = 0; i < 10; ++i) {
      m_pointers.push_back(new ExampleClass{i});
      m_integers.push_back(i * 2);
    }
  }

  ~Container() {
    for (auto p : m_pointers) {
      delete p;
    }
  }

  std::vector<ExampleClass *> GetPointers() {
    return m_pointers;
  }
  

  std::list<int> GetIntegers() {
    return m_integers;
  }

private:
  std::list<int> m_integers;
  std::vector<ExampleClass *> m_pointers;
};


float Random() {
  return 4.23f;
}

VMState *reentrancyhelper;
void ReentrancyTest(int value) {
  std::cout << "Return value from reentrancy: " << VMInstance().InvokeFunction(*reentrancyhelper, "foo", { VMValue{ value } }).ToString() << "\n";
}


int main() {


  try {
    LoggerManager::SetGlobalLogLevel(LogLevel::ALL);
    LoggerManager::SetLogFolder("logs");
    std::vector<VMState> states;
    states.push_back(std::move(Compiler::Compile("examples/scripttest.txt")));
   /* states.push_back(std::move(Compiler::Compile("examples/factorial.txt")));
    states.push_back(std::move(Compiler::Compile("examples/fizzbuzz.txt")));
    states.push_back(std::move(Compiler::Compile("examples/quicksort.txt")));
    states.push_back(std::move(Compiler::Compile("examples/garbage.txt")));*/


    LARGE_INTEGER StartingTime;
    LARGE_INTEGER EndingTime;
    LARGE_INTEGER ElapsedMicroseconds;
    LARGE_INTEGER Frequency;

    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);

 
   
    int counter = 1;

    Container container;
    for (auto &state : states) {


      reentrancyhelper = &state;
      std::cout << "Example " << counter++ << "\n\n";
      state.AddNativeBinding("printer", CreateBinding(&printer));
      state.AddNativeBinding("memberFunction", CreateBinding(&ExampleClass::ExampleFunction));
      state.AddNativeBinding("getpointers", CreateBinding(&Container::GetPointers));
      state.AddNativeBinding("getintegers", CreateBinding(&Container::GetIntegers));
      state.AddNativeBinding("random", CreateBinding(&Random));
      state.AddNativeBinding("reentrant", CreateBinding(&ReentrancyTest));

      auto value = VMInstance().InvokeFunction(state, "main", { VMValue{ 1 }, VMValue{ 2 }, VMValue{ &container } });
      std::cout << "\nReturn value was: ";
      printer(value);
      std::cout << "\n\n";
    }  

    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

    std::cout << "Time taken: " << ElapsedMicroseconds.QuadPart / 1000 << " milliseconds " << "\n";

  } catch (const std::exception &ex) {
    std::cout << "Exception:\n" << ex.what() << "\n";
  }




  std::cout << "Done executing\n";
  system("pause");
   
}