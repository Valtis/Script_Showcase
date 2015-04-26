#include "VM/Compiler/ByteCodePrinter/ByteCodePrinter.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMFunction.h"
#include "VM/Core/ByteCode.h"


void ByteCodePrinter::PrintByteCode(const VMState &state, const std::string path) {
  auto file_path = path + ".bytecode";
  m_output.open(file_path);
  if (!m_output.is_open()) {
    throw std::runtime_error("Failed to open file " + file_path + " for bytecode printing");
  }
  m_output << "Statics count: " << state.GetStaticObjectCount() << "\n\n";

  for (const auto &function : state.GetFunctions()) {
    PrintFunction(function, state.GetFunctionID(function.GetName()));
  }
}

void ByteCodePrinter::PrintFunction(const VMFunction &function, uint32_t id) {
  m_output << GetIdent() << "Function '" << function.GetName() << "' ID: " << id << "\n";
  m_output << "Local count: " << function.GetLocalCount() << "\n\n";

  for (m_pos = 0; m_pos < function.GetByteCodeCount(); ++m_pos) {
    m_output << GetIdent() << m_pos << ": " << GetByteCodeName(function.GetByteCode(m_pos));
    HandleArguments(function);
    m_output << "\n";    
  }
  DecreaseIdent();
  m_output << "\n\n";
}

/*
  Handles possible arguments for current bytecode. For example, PUSH_INTEGER has the integer value stored in the next bytecode.
*/
void ByteCodePrinter::HandleArguments(const VMFunction &function) {
  switch (function.GetByteCode(m_pos)) {
  case ByteCode::PUSH_INTEGER: 
    m_output << " " << static_cast<int32_t>(function.GetByteCode(++m_pos)); 
    break;
  case ByteCode::PUSH_FLOAT:
    {
      auto temp = static_cast<uint32_t>(function.GetByteCode(++m_pos));
      m_output << " " << *reinterpret_cast<float *>(&temp);
    }
    break;
  case ByteCode::PUSH_DOUBLE: 
  {
    auto highbits = static_cast<uint32_t>(function.GetByteCode(++m_pos));
    auto lowbits = static_cast<uint32_t>(function.GetByteCode(++m_pos));

    uint64_t val = 0;

    auto ptr = reinterpret_cast<char *>(&val);
    auto highPtr = reinterpret_cast<char *>(&highbits);
    auto lowPtr = reinterpret_cast<char *>(&lowbits);

    ptr[7] = highPtr[3];
    ptr[6] = highPtr[2];
    ptr[5] = highPtr[1];
    ptr[4] = highPtr[0];
    ptr[3] = lowPtr[3];
    ptr[2] = lowPtr[2];
    ptr[1] = lowPtr[1];
    ptr[0] = lowPtr[0];

    auto doubleVal = *reinterpret_cast<double *>(&val); 
    m_output << " " << doubleVal;
  }
    break;
  case ByteCode::PUSH_BOOLEAN:
    m_output << " " << (static_cast<int32_t>(function.GetByteCode(++m_pos)) == 0 ? "false" : "true");
    break;
  case ByteCode::LOAD_STATIC_OBJECT:
  case ByteCode::STORE_STATIC_OBJECT:
  case ByteCode::LOAD_LOCAL:
  case ByteCode::STORE_LOCAL:
  case ByteCode::INVOKE_MANAGED:
  case ByteCode::JUMP:
  case ByteCode::JUMP_IF_FALSE:
  case ByteCode::JUMP_IF_TRUE:
  case ByteCode::JUMP_IF_NEGATIVE:
  case ByteCode::JUMP_IF_POSITIVE:
  case ByteCode::JUMP_IF_ZERO:  
    m_output << " " << static_cast<uint32_t>(function.GetByteCode(++m_pos));
    break;
  default:
    break; // do nothing
  }
}

std::string ByteCodePrinter::GetIdent() const {
  std::string ident(m_ident*m_identPerLevel, '*');
  return ident;
}

void ByteCodePrinter::IncreaseIdent() {
  m_ident += m_identPerLevel;
}

void ByteCodePrinter::DecreaseIdent() {
  m_ident -= m_identPerLevel;
  if (m_ident < 0) {
    m_ident = 0;
  }
}