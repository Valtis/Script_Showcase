#include "VMFunction.h"

void VMFunction::AddByteCode(ByteCode code) {
  m_bytecode.push_back(code);
}

ByteCode VMFunction::GetByteCode(uint32_t pos) const {
  return m_bytecode.at(pos);
}


void VMFunction::SetName(const std::string &name) {
  m_name = name;
}

std::string VMFunction::GetName() const {
  return m_name;
}