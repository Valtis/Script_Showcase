#pragma once
#include "ByteCode.h"
#include <vector>
#include <cstdint>
#include <string>

class VMFunction {
public:

  void SetName(const std::string &name);
  std::string GetName() const;
  void VMFunction::AddByteCode(ByteCode code);
  ByteCode GetByteCode(uint32_t pos) const;
private:
  std::string m_name;
  std::vector<ByteCode> m_bytecode;

};
