#include "VM/Core/VMState.h"
#include "VM/Core/ByteCode.h"
#include "VM/Core/VM.h"
#include "VM/ScriptLoader/ScriptLoader.h"
VMState::VMState() {
  VMInstance().RegisterVMState(this);
}

VMState::VMState(VMState &&rhs) {
  DoMove(rhs);
}

VMState &VMState::operator=(VMState &&rhs) {
  if (&rhs != this) {
    DoMove(rhs);
  }

  return *this;
}

void VMState::DoMove(VMState &rhs) {
  VMInstance().UnregisterVMState(&rhs);

  m_functioNameToIndexMapping = std::move(rhs.m_functioNameToIndexMapping);
  m_functions = std::move(rhs.m_functions);
  m_native_bindings = std::move(rhs.m_native_bindings);
  m_static_objects = std::move(rhs.m_static_objects);

  VMInstance().RegisterVMState(this);
}

VMState::~VMState() {
  VMInstance().UnregisterVMState(this);
}



size_t VMState::AddFunction(VMFunction function)
{
  m_functioNameToIndexMapping[function.GetName()] = m_functions.size();
  m_functions.push_back(function);
  return m_functioNameToIndexMapping[function.GetName()];
}


const VMFunction *VMState::GetFunction(uint32_t index) const
{
  try {
    return &m_functions.at(index);
  } catch (const std::out_of_range &ex) {
    throw std::logic_error("Internal error: No function with index " + std::to_string(index) + " found");
  }
}

const VMFunction *VMState::GetFunction(const std::string &name) const
{
  auto it = m_functioNameToIndexMapping.find(name);
  if (it == m_functioNameToIndexMapping.end()) {
    return nullptr;
  }
  return GetFunction(it->second);
}

void VMState::SetStaticObject(uint32_t index, VMValue value) {
  m_static_objects.at(index) = value;
}

VMValue VMState::GetStaticObject(uint32_t index) const {
  return m_static_objects.at(index);
}

VMValue &VMState::GetStaticObjectReference(uint32_t index) {
  return m_static_objects.at(index);
}

size_t VMState::AddStaticObject(VMValue value) {
  m_static_objects.push_back(value);
  return m_static_objects.size() - 1;
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

size_t VMState::GetStaticObjectCount() const {
  return m_static_objects.size();
}