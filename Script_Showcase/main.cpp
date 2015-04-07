#include <iostream>
#include "VM/Utility/LoggerManager.h"
#include "VM/Core/VM.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMValue.h"
#include <vector>

void VMLogger(VMValue value) {
  LoggerManager::GetLog(SCRIPT_LOG).AddLine(LogLevel::INFO, "Topmost value in stack: " + value.ToString());
}

int main() {
  LoggerManager::SetLogFolder("/logs");
  LoggerManager::SetGlobalLogLevel(LogLevel::ALL);
  
  std::vector<VMState> states;
  states.push_back(std::move(VMState{"examples/examples.txt" }));


  for (auto &state : states) {
    VMInstance().InvokeFunction(state, "example", {});
  }
 

  
  int dummy;
  std::cin >> dummy; 
}