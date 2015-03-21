#include "VM.h"
#include "VMState.h"
#include "FFI/ConversionFunctions.h"
#include "FFI/NativeBinding.h"
#include "VMOperations.h"

#include <iostream>


const uint32_t stackSize = 4096;
const uint32_t frameSize = 1024;
VM::VM() {

  m_stack.reserve(stackSize);
  m_frames.reserve(frameSize);
}

// needs to be broken into smaller functions.
VMValue VM::InvokeFunction(VMState &state, const std::string &functionName, std::vector<VMValue> objects) {
  
  auto function = state.GetFunction(functionName);
  if (function == nullptr) {
    return{};
  }
 
  InitializeVMForExecution(functionName, objects, function);
  
  try {
    Execute(state);
  } catch (const std::exception &ex) {
    BuildStackTraceAndThrow(ex);
  }

  return ReturnValue();

}

void VM::InitializeVMForExecution(const std::string & functionName, std::vector<VMValue> objects, const VMFunction *function)
{
  m_stack.clear();
  m_frames.clear();

  m_stack.reserve(stackSize);
  m_frames.reserve(frameSize);

  m_frames.push_back(VMFrame{ function });

  for (const auto &o : objects) {
    m_stack.push_back(o);
  }
}


void VM::Execute(VMState &state) {

  for (;;)  {
    auto code = m_frames.back().GetNextInstruction();

    switch (code) {

    case ByteCode::PUSH_INTEGER:
      Op::PushInteger(m_stack, m_frames);
      break; 
    case ByteCode::PUSH_CONSTANT_OBJECT: 
      Op::PushConstantObject(state, m_stack, m_frames);
      break;

    case ByteCode::ADD_INTEGER:
      Op::AddInteger(m_stack);
      break;
    
    case ByteCode::SUB_INTEGER:
      Op::SubInteger(m_stack);
      break;    
    case ByteCode::MUL_INTEGER:
      Op::MulInteger(m_stack);
      break;
    case ByteCode::DIV_INTEGER:
      Op::DivInteger(m_stack);
      break;

    case ByteCode::INVOKE_NATIVE:
      Op::InvokeNative(state, m_stack);
      break;
    case ByteCode::INVOKE_MANAGED:
      Op::InvokeManaged(state, m_stack, m_frames);
      break;

    case ByteCode::RETURN:
      if (!Op::Return(m_frames)) {
        return;
      }
      break;
    default: 
    {
      std::string error = std::string("Fatal error: Unrecognized byte code: ")
        + std::to_string((uint32_t)code);

      std::cout << "Error:" <<  error << "\n";
      throw std::runtime_error(error);
    }
    }
  }
}

VMValue VM::ReturnValue() {
  // return topmost stack item, if any
  if (!m_stack.empty()) {
    return m_stack.back();
  }
  else {
    return{};
  }
}

void VM::BuildStackTraceAndThrow(const std::exception &ex) {

  std::string error = std::string("An error has occurred during script execution: ") + ex.what();
  std::string stack_trace = "\n" + error + "\n";
 
  AddBasicScriptInfoToErrorMessage(stack_trace);
  AddFrameStackToErrorMessage(stack_trace);
  AddValueStackToErrorMessage(stack_trace);


  std::cout << stack_trace << "\n";

  throw std::runtime_error(error);
}

void VM::AddBasicScriptInfoToErrorMessage(std::string &stack_trace) {
  auto &frame = m_frames.back();
  stack_trace += "\tWhen executing script function '" + frame.GetFunctionName() + "'\n";
  stack_trace += "\tWhen executing instruction '" + std::string(GetByteCodeName(frame.GetPreviousInstruction())) + "'\n";
  stack_trace += "\tWith program counter value " + std::to_string(frame.GetProgramCounter()) + "\n\n";
}

void VM::AddFrameStackToErrorMessage(std::string &stack_trace) {
  for (int i = m_frames.size() - 2; i >= 0; --i) {
    stack_trace += "\t\tCalled from script function '" + m_frames[i].GetFunctionName() + "'\n";
    stack_trace += "\t\tWith program counter value " + std::to_string(m_frames[i].GetProgramCounter()) + "\n\n";
  }
}

void VM::AddValueStackToErrorMessage(std::string &stack_trace) {
  stack_trace += "Script stack:\n\n";
  for (int i = m_stack.size() - 1; i >= 0; --i) {
    stack_trace += std::to_string(i) + ": " + m_stack[i].to_string() + "\n";
  }
}
