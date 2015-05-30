#pragma once

#include "VM/Core/VMValue.h"
#include "VM/Core/VMFunction.h"
#include <unordered_map>
#include <vector>
#include <string>

#include "VM/FFI/NativeBindingTypedef.h"

class VMFunction;

/*
  Script file is compiled into VMState file; VMState thus represents single file. This allows multiple different 
  files to have functions with same names (update, initialize). Contains all the functions.

  This class also stores static variables whose values survive between function calls; VM stacks are reset whenever InvokeFunction is called, thus
  any values in local variables are lost. 

  This class also stores native bindings, which allow script functions to call C++ functions.

  As any instance of this class is registered with VM as a potential source of pointers for GC, this class is not copyable in order to prevent any accidental 
  pointer invalidations. Move constructor\assignment operator will update registrations if needed

*/

class VMState {
public:

  VMState();
  VMState(const VMState &) = delete;
  VMState(VMState &&);
  VMState &operator=(const VMState &) = delete;
  VMState &operator=(VMState &&);
  ~VMState();
  /*
    Important note: It is not safe to call this function once GetFunction is called, as the backing container is a vector and any outstanding pointers
    might get invalidated. 
  */
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