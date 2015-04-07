#pragma once
#include "VM/Core/ByteCode.h"
#include <vector>
#include <cstdint>
#include <string>

class VMFunction {
public:

  void SetName(const std::string &name);
  std::string GetName() const;
  size_t VMFunction::AddByteCode(ByteCode code);
  ByteCode GetByteCode(uint32_t pos) const;

  void ChangeByteCode(size_t index, ByteCode code);
  size_t GetByteCodeCount();
  void SetLocalCount(size_t locals);
  size_t GetLocalCount() const;
private:
  std::string m_name;
  std::vector<ByteCode> m_bytecode;
  size_t m_localCount;
};
