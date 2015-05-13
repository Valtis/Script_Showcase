#pragma once

#include "VM/Memory/RootsetProvider.h"
#include "VM/Core/VMValue.h"
#include "VM/Core/VMFrame.h"
#include <vector>
#include <unordered_set>
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
  void RegisterVMState(VMState *state) { m_states.insert(state);  }
  void UnregisterVMState(VMState *state) { m_states.erase(state); }
private:

  void SaveState();
  void RestoreOldContext();

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
  std::unordered_set<VMState *> m_states;

  // used to store previous execution context in case we are re-entering the vm
  std::vector<std::vector<VMValue>> m_previousStacks;
  std::vector<std::vector<VMFrame>> m_previousFrames;

};



// VM singleton. Allows code to invoke VM where needed
VM &VMInstance();