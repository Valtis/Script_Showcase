#pragma once
#include "VM/Core/ByteCode.h"
#include <vector>
#include <cstdint>
#include <string>

/*
  Contains function name, function local count, function argument count and bytecode.  
*/
class VMFunction {
public:
  VMFunction();
  void SetName(const std::string &name);
  std::string GetName() const;
  size_t VMFunction::AddByteCode(ByteCode code);
  ByteCode GetByteCode(uint32_t pos) const;

  void ChangeByteCode(size_t index, ByteCode code);
  size_t GetByteCodeCount() const;
  
  void SetLocalCount(size_t locals);
  size_t GetLocalCount() const;

  void SetArgumentCount(size_t arguments);
  size_t GetArgumentCount() const;

private:
  std::string m_name;
  std::vector<ByteCode> m_bytecode;
  size_t m_localCount;
  size_t m_argumentCount;
};
