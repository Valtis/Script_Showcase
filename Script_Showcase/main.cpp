#include <iostream>
#include "VM/Utility/LoggerManager.h"
#include "VM/Core/VM.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMValue.h"
#include "VM/FFI/NativeBinding.h"
#include "VM/Compiler/Compiler.h"
#include "VM/Memory/MemoryManager.h"
#include <vector>

#include <windows.h>
void printer(VMValue value) {
  // really stupid and hacky way of resolving this.
  if (value.GetType() == ValueType::MANAGED_POINTER && MemMgrInstance().IsArray(value) && MemMgrInstance().GetArrayType(value) == ValueType::CHAR) {
    std::cout << ToNativeType<std::string>(value) << "\n";
  } else {
    std::cout << value.ToString() << "\n";
  }
}

class ExampleClass
{
public:
  ExampleClass(int value) : m_value(value) {

  }

  void ExampleFunction(int fromVm) {
    std::cout << "My number is: " << m_value << " and vm value is: " << fromVm << "\n";
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
      auto value = VMInstance().InvokeFunction(state, "example", { VMValue(1234.567), VMValue(8775) });
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