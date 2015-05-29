#include "VM/Core/VMFrame.h"

VMFrame::VMFrame() : VMFrame(nullptr) { }

VMFrame::VMFrame(const VMFunction *function) : m_function(function), m_current_instruction(0) {
  if (function != nullptr) {
    m_localVariables.resize(function->GetLocalCount());
  }
}

ByteCode VMFrame::GetNextInstruction() {
  return m_function->GetByteCode(m_current_instruction++);
}

ByteCode VMFrame::GetPreviousInstruction() const {
  return m_function->GetByteCode(m_current_instruction - 1);
}

void VMFrame::SetNextInstruction(uint32_t instruction) {
  m_current_instruction = instruction;
}


std::string VMFrame::GetFunctionName() const {
  return m_function->GetName();
}

uint32_t VMFrame::GetProgramCounter() const {
  return m_current_instruction;
}

void VMFrame::SetLocalVariable(size_t index, VMValue value) {
  m_localVariables.at(index) = value;
}

VMValue VMFrame::GetLocalVariable(size_t index) const {
  return m_localVariables.at(index);
}

VMValue &VMFrame::GetLocalVariableReference(size_t index) {
  return m_localVariables.at(index);
}
size_t VMFrame::GetLocalVariableCount() const {
  return m_localVariables.size();
}