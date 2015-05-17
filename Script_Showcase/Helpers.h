#pragma once
#include "VM/Core/VMValue.h"
#include <vector>
#include <list>
#include "VM/Core/VMState.h"
/*
  Contains functions and classes that will be made available to the scripts
*/
void printer(VMValue value);

float Random();

void AddBindings(std::vector<VMState> &states);

class ExampleClass
{
public:
  ExampleClass(int value);

  void ExampleFunction(int fromVm);
  bool ExampleFunction2(int fromVm, int fromVm2);

private:
  int m_value;
};

class Container {
public:
  Container();
  ~Container();

  std::vector<ExampleClass *> GetPointers();
  std::list<int> GetIntegers();

private:
  std::list<int> m_integers;
  std::vector<ExampleClass *> m_pointers;
};

