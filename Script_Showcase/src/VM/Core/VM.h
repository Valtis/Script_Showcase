#pragma once

#include "VM/Memory/RootsetProvider.h"
#include "VM/Core/VMValue.h"
#include "VM/Core/VMFrame.h"
#include <vector>
#include <algorithm>

class VMState;

/*
  Main VM class. Handles fetch/encode/execute cycle together with functions in VMOperations.h/cpp. Contains operand and function 
  frame stacks.
  
  Also implements RootSetProvider interface and thus this class is responsible for scanning stacks and VMStates for potential root objects

  TODO: Too many responsibilities, should be split into smaller classes
*/

class VM : public RootSetProvider {
public:
  VM();
  VMValue InvokeFunction(VMState &state, const std::string &functionName, std::vector<VMValue> arguments);
 
  std::vector<VMValue *> GetRootSet() override;
  
  // (un)registers VMState as potential source of pointers for garbage collection. 
  void RegisterVMState(VMState *state) { m_states.push_back(state);  }
  void UnregisterVMState(VMState *state) { m_states.erase(std::remove(std::begin(m_states), std::end(m_states), state), std::end(m_states)); }
private:

  void InitializeVMForExecution(const std::string & functionName, std::vector<VMValue> arguments, const VMFunction *function);
  void Execute(VMState &state);
  VMValue ReturnValue();
  void BuildStackTraceAndThrow(const std::exception &ex);
  std::string CreateStackTrace();
  void AddBasicScriptInfoToErrorMessage(std::string &stack_trace);
  void AddValueStackToErrorMessage(std::string &stack_trace);
  void AddFrameStackToErrorMessage(std::string &stack_trace);

  void GetRootsFromStack(std::vector<VMValue *> &rootSet);
  void GetRootsFromFrames(std::vector<VMValue *> &rootSet);
  void GetRootsFromStates(std::vector<VMValue *> &rootSet);

  std::vector<VMValue> m_stack;
  std::vector<VMFrame> m_frames;
  std::vector<VMState *> m_states;

};



// VM singleton. Allows code to invoke VM where needed
VM &VMInstance();