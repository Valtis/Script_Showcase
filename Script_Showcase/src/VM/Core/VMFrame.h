#pragma once
#include "VM/Core/VMFunction.h"
#include "VM/Core/VMValue.h"
#include <cstdint>
class VMFrame {

public:
  VMFrame();
  VMFrame(const VMFunction *function);
  ByteCode GetNextInstruction();
  ByteCode GetPreviousInstruction() const;
  void SetNextInstruction(uint32_t);
  std::string GetFunctionName() const;
  uint32_t GetProgramCounter() const;

  void SetLocalVariable(size_t index, VMValue value);
  VMValue GetLocalVariable(size_t index) const;
  VMValue &GetLocalVariableReference(size_t index);

  size_t GetLocalVariableCount() const;
private:
  std::vector<VMValue> m_localVariables;
  const VMFunction *m_function; // non-owning pointer - pointed value must not be deleted
  uint32_t m_current_instruction;
};