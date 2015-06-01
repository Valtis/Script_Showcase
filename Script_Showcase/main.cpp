#include <iostream>
#include "Utility/LoggerManager.h"
#include "VM/Core/VM.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMValue.h"
#include "VM/Compiler/Compiler.h"
#include "VM/Exceptions/Exceptions.h"
#include <vector>

#include <windows.h>
#include "Helpers.h"


std::vector<VMState> LoadScripts() {

  std::vector<VMState> states;
  states.push_back(std::move(Compiler::Compile("examples/factorial.txt")));
  states.push_back(std::move(Compiler::Compile("examples/fizzbuzz.txt")));
  states.push_back(std::move(Compiler::Compile("examples/quicksort.txt")));
  states.push_back(std::move(Compiler::Compile("examples/functionPassing.txt")));  
  states.push_back(std::move(Compiler::Compile("examples/tailrecursion.txt")));
  states.push_back(std::move(Compiler::Compile("examples/garbage.txt")));
  
  AddBindings(states);

  return states;
}

class Foo {
  
};


int main() {


  try {
    LoggerManager::SetGlobalLogLevel(LogLevel::ALL);
    LoggerManager::SetLogFolder("logs");



 
    auto states = LoadScripts();
    int counter = 1;

    Container container;
    Foo foo;
    LARGE_INTEGER StartingTime;
    LARGE_INTEGER EndingTime;
    LARGE_INTEGER ElapsedMicroseconds;
    LARGE_INTEGER Frequency;

    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);
    for (auto &state : states) {
      std::cout << "Example " << counter++ << "\n\n";
      auto value = VMInstance().InvokeFunction(state, "main", { VMValue{ 1 }, VMValue{ &foo }, VMValue{ &container } });
      std::cout << "\nReturn value was: ";
      printer(value);
      std::cout << "\n\n";
    }  

    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

    std::cout << "Time taken: " << ElapsedMicroseconds.QuadPart / 1000 << " milliseconds " << "\n";

  }
  catch (const std::exception &ex) {
    std::cout << "Exception:\n" << ex.what() << "\n";
  }

  
  std::cout << "Done executing\n";
  system("pause");
   
}