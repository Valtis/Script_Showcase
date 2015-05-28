#include "VM/Core/VMOperations.h"
#include "VM/Core/VM.h"
#include "VM/Core/VMState.h"
#include "Utility/LoggerManager.h"

VM::VM() {
  m_stack.reserve(STACK_SIZE);
  m_frames.reserve(FRAME_SIZE);
}

VMValue VM::InvokeFunction(VMState &state, const std::string &functionName, std::vector<VMValue> arguments) {
  
  auto function = state.GetFunction(functionName);
  if (function == nullptr) {
    return{};
  }

  InitializeVMForExecution(functionName, arguments, function);
  
  try {
    Execute(state);
  } catch (const std::exception &ex) {
    BuildStackTraceAndThrow(ex);
  }

  auto retVal = ReturnValue();
  RestoreOldContext(); 
  return retVal;
}

void VM::InitializeVMForExecution(const std::string & functionName, std::vector<VMValue> arguments, const VMFunction *function)
{
  if (function->GetArgumentCount() != arguments.size()) {
    throw std::runtime_error("Invalid argument count when invoking script function: " + std::to_string(arguments.size()) +
      " arguments were provided but " + std::to_string(function->GetArgumentCount()) + " arguments were expected");
  }

  // we are re-entering the vm: save old stack and frame values
  if (m_stack.size() != 0 || m_frames.size() != 0) {
    SaveState();
  }

  m_stack.clear();
  m_frames.clear();
  
  m_frames.push_back(VMFrame{ function });

  for (const auto &o : arguments) {
    m_stack.push_back(o);
  }
}

void VM::SaveState() {
  m_previousStacks.push_back(std::move(m_stack));
  m_previousFrames.push_back(std::move(m_frames));
}


void VM::Execute(VMState &state) {

  for (;;)  {
    auto code = m_frames.back().GetNextInstruction();

    switch (code) {
    case ByteCode::POP:
      Op::PopValue(m_stack);
    break;
    case ByteCode::PUSH_INTEGER:
      Op::PushInteger(m_stack, m_frames);
      break;
    case ByteCode::PUSH_FLOAT:
      Op::PushFloat(m_stack, m_frames);
      break;
    case ByteCode::PUSH_DOUBLE:
      Op::PushDouble(m_stack, m_frames);
      break;
    case ByteCode::PUSH_BOOLEAN:
      Op::PushBoolean(m_stack, m_frames);
      break;
    case ByteCode::PUSH_FUNCTION:
      Op::PushFunction(m_stack, m_frames);
      break;
    case ByteCode::LOAD_LOCAL:
      Op::LoadLocal(m_stack, m_frames);
      break;
    case ByteCode::STORE_LOCAL:
      Op::StoreLocal(m_stack, m_frames);
      break;
    case ByteCode::LOAD_STATIC_OBJECT: 
      Op::LoadStaticObject(state, m_stack, m_frames);
      break;
    case ByteCode::STORE_STATIC_OBJECT:
      Op::StoreStaticObject(state, m_stack, m_frames);
      break;
    case ByteCode::LOAD_ARRAY_INDEX:
      Op::LoadArrayIndex(m_stack);
      break;
    case ByteCode::STORE_ARRAY_INDEX:
      Op::StoreArrayIndex(m_stack);
      break;
    case ByteCode::ARRAY_LENGTH:
      Op::ArrayLength(m_stack);
      break;
    case ByteCode::IS_GREATER:   
      Op::IsGreater(m_stack);
      break;
    case ByteCode::IS_GREATER_OR_EQ: 
      Op::IsGreaterOrEq(m_stack);
      break;
    case ByteCode::IS_EQ: 
      Op::IsEq(m_stack);
      break;
    case ByteCode::IS_LESS_OR_EQ: 
      Op::IsLessOrEq(m_stack);
      break;
    case ByteCode::IS_LESS: 
      Op::IsLess(m_stack);
      break;
    case ByteCode::JUMP:
      Op::Jump(state, m_stack, m_frames);
      break;
    case ByteCode::JUMP_IF_TRUE: 
      Op::JumpIfTrue(state, m_stack, m_frames);
      break;
    case ByteCode::JUMP_IF_FALSE: 
      Op::JumpIfFalse(state, m_stack, m_frames);
      break;
    case ByteCode::JUMP_IF_ZERO:
      Op::JumpIfZero(state, m_stack, m_frames);
      break;
    case ByteCode::JUMP_IF_NEGATIVE:
      Op::JumpIfNegative(state, m_stack, m_frames);
      break; 
    case ByteCode::JUMP_IF_POSITIVE:
      Op::JumpIfPositive(state, m_stack, m_frames);
      break;
    case ByteCode::ADD:
      Op::Add(m_stack);
      break;
    case ByteCode::SUB:
      Op::Sub(m_stack);
      break;
    case ByteCode::MUL:
      Op::Mul(m_stack);
      break;
    case ByteCode::DIV:
      Op::Div(m_stack);
      break;
    case ByteCode::MOD:
      Op::Mod(m_stack);
      break;
    case ByteCode::NOT:
      Op::Not(m_stack);
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
    case ByteCode::INVOKE_MANAGED_INDIRECT:
      Op::InvokeManagedIndirect(state, m_stack, m_frames);
      break;
    case ByteCode::ALLOCATE_INTEGER_ARRAY:
      Op::AllocateIntegerArray(m_stack);
      break; 
    case ByteCode::ALLOCATE_OBJECT_ARRAY:
      Op::AllocateObjectArray(m_stack);
      break;
    case ByteCode::DOUBLE_TO_INTEGER:
      Op::DoubleToInteger(m_stack);
      break;
    case ByteCode::RETURN:
      if (!Op::Return(m_frames)) {
        return;
      }
      break;
    case ByteCode::NOP:
      break;
    default: 
    {
      std::string error = std::string("Fatal error: Unrecognized byte code: ")
        + std::to_string((uint32_t)code);

      LoggerManager::GetLog(VM_LOG).AddLine(LogLevel::ERROR, error);
      throw std::runtime_error(error);
    }
    }
  }
}


void VM::RestoreOldContext() {
  if (m_previousStacks.size() != 0) {
    m_stack = std::move(m_previousStacks.back());
    m_previousStacks.pop_back();
  }
  
  if (m_previousFrames.size() != 0) {
    m_frames = std::move(m_previousFrames.back());
    m_previousFrames.pop_back();
  }
}


VMValue VM::ReturnValue() {
  // return topmost stack item, if any
  if (m_stack.empty()) {
    return{};
  }
  auto value = m_stack.back();
  m_stack.pop_back();
  return value;
}

void VM::BuildStackTraceAndThrow(const std::exception &ex) {

  auto &log = LoggerManager::GetLog(VM_LOG);
  std::string error = std::string("An error has occurred during script execution: ") + ex.what();
  std::string stack_trace = "\n" + error + "\n";
 
  stack_trace += CreateStackTrace();


  log.AddLine(LogLevel::ERROR, stack_trace);

  throw std::runtime_error(error);
}

std::string VM::CreateStackTrace() {
  std::string stackTrace;
  AddBasicScriptInfoToErrorMessage(stackTrace);
  AddFrameStackToErrorMessage(stackTrace);
  AddValueStackToErrorMessage(stackTrace);
  return stackTrace;
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
    stack_trace += std::to_string(i) + ": " + m_stack[i].ToString() + "\n";
  }
}

std::vector<VMValue *> VM::GetRootSet()  {
  std::vector<VMValue *> rootSet;
  GetRootsFromStack(rootSet);
  GetRootsFromFrames(rootSet);
  GetRootsFromStates(rootSet);

  return rootSet;
}

void VM::GetRootsFromStack(std::vector<VMValue *> &rootSet)  {
  for (auto &value : m_stack) {
    if (value.GetType() == ValueType::MANAGED_POINTER) {
      rootSet.push_back(&value);
    }
  }
}

void VM::GetRootsFromFrames(std::vector<VMValue *> &rootSet) {
  for (auto &frame : m_frames) {
    for (size_t i = 0; i < frame.GetLocalVariableCount(); ++i) {
      if (frame.GetLocalVariableReference(i).GetType() == ValueType::MANAGED_POINTER) {
        rootSet.push_back(&frame.GetLocalVariableReference(i));
      }
    }
  }
}

void VM::GetRootsFromStates(std::vector<VMValue *> &rootSet) {
  for (auto state : m_states) {
    for (size_t i = 0; i < state->GetStaticObjectCount(); ++i) {
      VMValue *object = &state->GetStaticObjectReference(i);
      if (object->GetType() == ValueType::MANAGED_POINTER) {
        rootSet.push_back(object);
      }
    }
  }
}







VM &VMInstance() {
  static VM vm;
  return vm;
}
