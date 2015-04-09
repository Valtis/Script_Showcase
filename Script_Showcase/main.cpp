#include <iostream>
#include "VM/Utility/LoggerManager.h"
#include "VM/Core/VM.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMValue.h"
#include "VM/FFI/NativeBinding.h"
#include "VM/Compiler/Compiler.h"
#include <vector>

void printer(VMValue value) {
  std::cout << "Topmost value in stack: " + value.ToString() << "\n";
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

    int counter = 1;
    for (auto &state : states) {
      std::cout << "Example " << counter++ << "\n\n";
      state.AddNativeBinding("printer", CreateBinding(&printer));
      state.AddNativeBinding("memberFunction", CreateBinding(&ExampleClass::ExampleFunction));
      VMInstance().InvokeFunction(state, "example", { VMValue(&exampleObject1), VMValue(&exampleObject2) });
      std::cout << "\n\n";
    }

  } catch (const std::exception &ex) {
    std::cout << "Exception: " << ex.what() << "\n";
  }


  std::cout << "Done executing\n";
  system("pause");
   
}