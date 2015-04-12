#include "VM/Core/VMFunction.h"

VMFunction::VMFunction() : m_localCount(0), m_argumentCount(0) {

}

size_t VMFunction::AddByteCode(ByteCode code) {
  m_bytecode.push_back(code);
  return m_bytecode.size() - 1;
}

void VMFunction::ChangeByteCode(size_t index, ByteCode code) {
  m_bytecode.at(index) = code;
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

size_t VMFunction::GetByteCodeCount() {
  return m_bytecode.size();
}
void VMFunction::SetLocalCount(size_t locals) {
  m_localCount = locals;
}

size_t VMFunction::GetLocalCount() const {
  return m_localCount;
}


void VMFunction::SetArgumentCount(size_t arguments) {
  m_argumentCount = arguments;
}
size_t VMFunction::GetArgumentCount() const {
  return m_argumentCount;
}
