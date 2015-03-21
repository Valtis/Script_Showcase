#include "VMState.h"
#include "ByteCode.h"
#include "VM.h"
#include "MemoryManager.h"
VMState::VMState(const std::string &path) {
  LoadByteCodeFile(path);
}



void VMState::LoadByteCodeFile(const std::string &path) {
  // TODO: Add logic that loads files
}


void VMState::AddFunction(const std::string &name, const VMFunction &function) {
  m_functions[name] = function;
}

const VMFunction *VMState::GetFunction(const std::string &name) const {
  auto iter = m_functions.find(name);
  if (iter == m_functions.end()) {
    return nullptr;
  }

  return &iter->second;
}



uint32_t VMState::AddPermanentStorageObject(VMValue value) {
  m_permanent_storage.push_back(value);
  return m_permanent_storage.size() - 1;
}
VMValue VMState::GetPermanentStorageObject(uint32_t index) const {
  return m_permanent_storage.at(index);
}

NativeBinding VMState::GetNativeBinding(const std::string &name) const {
  auto bindIter = m_native_bindings.find(name);
  if (bindIter == m_native_bindings.end()) {
    throw std::runtime_error(std::string("No native binding with name ") + name + " has been registered");
  }
  return bindIter->second;
}

void VMState::AddNativeBinding(const std::string &name, NativeBinding binding) {
  m_native_bindings[name] = binding;
}