#include <iostream>
#include "VM/Utility/LoggerManager.h"
#include "VM/Core/VM.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMValue.h"
#include "VM/FFI/NativeBinding.h"
#include "VM/Compiler/Compiler.h"
#include "VM/Memory/MemoryManager.h"
#include <vector>
#include <cstdio>

#include <windows.h>
void printer(VMValue value) {
  // really stupid and hacky way of resolving this.
  if (value.GetType() == ValueType::MANAGED_POINTER && MemMgrInstance().IsArray(value) && MemMgrInstance().GetArrayType(value) == ValueType::CHAR) {
   printf("%s", ToNativeType<std::string>(value).c_str());
  } else {
    switch (value.GetType()) {
    case ValueType::BOOL: printf("%d", value.AsBool()); break;
    case ValueType::CHAR: printf("%c", value.AsChar()); break;
    case ValueType::DOUBLE: printf("%f",value.AsDouble()); break;
    case ValueType::FLOAT: printf("%f", value.AsFloat()); break;
    case ValueType::INT: printf("%d", value.AsInt()); break;
    case ValueType::MANAGED_POINTER:
    case ValueType::NATIVE_POINTER:
      std::cout << value.ToString();
      break;
    }
  }
  std::cout << "\n";
}


double multiply(int a, int b) {
  return a*b;
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

int main() {

  try {
 
    ExampleClass exampleObject1(1234);
    ExampleClass exampleObject2(567);

    LoggerManager::SetGlobalLogLevel(LogLevel::ALL);
    LoggerManager::SetLogFolder("logs");
    std::vector<VMState> states;
    states.push_back(std::move(Compiler::Compile("examples/scripttest.txt")));
    
    LARGE_INTEGER StartingTime;
    LARGE_INTEGER EndingTime;
    LARGE_INTEGER ElapsedMicroseconds;
    LARGE_INTEGER Frequency;

    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);

 
   
    int counter = 1;
    for (auto &state : states) {
      std::cout << "Example " << counter++ << "\n\n";
      state.AddNativeBinding("printer", CreateBinding(&printer));
      state.AddNativeBinding("memberFunction", CreateBinding(&ExampleClass::ExampleFunction));
      state.AddNativeBinding("memberFunction2", CreateBinding(&ExampleClass::ExampleFunction2));
      state.AddNativeBinding("multiply", CreateBinding(&multiply));
      auto value = VMInstance().InvokeFunction(state, "example", { VMValue{ 1234.567 }, VMValue{ 8775 }, VMValue{&exampleObject1} });
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
    std::cout << "Exception: " << ex.what() << "\n";
  }




  std::cout << "Done executing\n";
  system("pause");
   
}