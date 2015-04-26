#pragma once
#include <string>
#include <fstream>
#include <cstdint>
class VMState;
class VMFunction;

/*
  This class prints given VMState contents (functions & bytecode) to FILE_PATH.bytecode -file.
  This is mostly intended for easier bytecode inspection in case of broken code gen.
*/
class ByteCodePrinter {
public:
  void PrintByteCode(const VMState &state, const std::string path);
private:
  void PrintFunction(const VMFunction &function, uint32_t id);
  void HandleArguments(const VMFunction& function);
  std::string m_path;
  std::ofstream m_output;
  uint32_t m_pos;  
  int32_t m_ident;

  std::string GetIdent() const;
  void IncreaseIdent();
  void DecreaseIdent();
  const int32_t m_identPerLevel = 4;
};