#pragma once
#include <vector>

/*
  Interface for root set providing classes. 
  Any implementing class must be capable of returning all live pointers in stack, local variables and static variables.
  
  Used by garbage collector algorithms, which use this to get the initial set of pointers.
*/

class VMValue;
class RootSetProvider {
public:
  virtual ~RootSetProvider();
  virtual std::vector<VMValue *> GetRootSet() = 0;
};