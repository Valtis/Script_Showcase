#include "VMFrame.h"

VMFrame::VMFrame() : VMFrame(nullptr) { }

VMFrame::VMFrame(const VMFunction *function) : m_function(function), m_current_instruction(0) {

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