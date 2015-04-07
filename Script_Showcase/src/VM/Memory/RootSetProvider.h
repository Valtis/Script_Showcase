#pragma once
#include <vector>
class VMValue;
class RootSetProvider {
public:
  virtual ~RootSetProvider();
  virtual std::vector<VMValue *> GetRootSet() = 0;
};