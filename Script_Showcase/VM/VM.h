#pragma once

#include "VMValue.h"
#include "VMFrame.h"
#include "VMClass.h"
#include <array>
#include <vector>
#include <cstdint>
#include <string>


class VMState;

class VM {
public:
  VM();
  VMValue InvokeFunction(VMState &state, const std::string &functionName, std::vector<VMValue> objects);




private:

  void InitializeVMForExecution(const std::string & functionName, std::vector<VMValue> objects, const VMFunction *function);
  void Execute(VMState &state);
  VMValue ReturnValue();
  void BuildStackTraceAndThrow(const std::exception &ex);

  void AddBasicScriptInfoToErrorMessage(std::string &stack_trace);
  void AddValueStackToErrorMessage(std::string &stack_trace);
  void AddFrameStackToErrorMessage(std::string &stack_trace);

  std::vector<VMValue> m_stack;
  std::vector<VMFrame> m_frames;

};
