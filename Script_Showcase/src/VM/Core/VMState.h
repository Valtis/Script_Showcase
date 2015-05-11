#pragma once

#include "VM/Core/VMValue.h"
#include "VM/Core/VMFunction.h"
#include <unordered_map>
#include <vector>
#include <string>

#include "VM/FFI/NativeBindingTypedef.h"

class VMFunction;

class VMState {
public:

  VMState();
  VMState(const VMState &) = delete;
  VMState(VMState &&);
  VMState &operator=(const VMState &) = delete;
  VMState &operator=(VMState &&);
  ~VMState();

  size_t AddFunction(VMFunction function);
  const VMFunction *GetFunction(uint32_t index) const; 
  uint32_t GetFunctionID(const std::string &name) const;
  const VMFunction *GetFunction(const std::string &name) const;
  const std::vector<VMFunction> &GetFunctions() const;
  std::vector<VMFunction> &GetMutableFunctions();

  void SetStaticObject(uint32_t index, VMValue value);
  VMValue GetStaticObject(uint32_t index) const;
  VMValue &GetStaticObjectReference(uint32_t index);
  size_t AddStaticObject(VMValue value);
  
  NativeBinding GetNativeBinding(const std::string &name) const;
  void AddNativeBinding(const std::string &name, NativeBinding binding);

  size_t GetStaticObjectCount() const;
private:
  std::unordered_map<std::string, size_t> m_functioNameToIndexMapping;
  std::vector<VMFunction> m_functions;
  std::unordered_map<std::string, NativeBinding> m_native_bindings;
  std::vector<VMValue> m_static_objects;


  void DoMove(VMState &rhs);
};