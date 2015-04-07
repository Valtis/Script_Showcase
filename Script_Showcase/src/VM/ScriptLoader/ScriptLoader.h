#pragma once
#include "ByteCodeCreator/ByteCodeCreator.h"
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <unordered_map>

class VMFunction;
class VMState;
class ScriptLoader {
public:
  ScriptLoader(VMState &state, std::string path);

  void Load();
private:
  void LoadStatics();
  void LoadFunctions();
  void LoadFunction(const std::string &name);
  bool GetLine(std::string &line);
  void AddStaticValue(const std::vector<std::string> &tokens);
  std::unordered_map<std::string, size_t> LoadLocals();

  // handle jumps that jump forward. The jump label was not declared at the time of the jump, so we check for these after everything has been parsed
  void HandleJumpsWithUndeclaredTargets(VMFunction &function, std::unordered_map<std::string, size_t> &unhandledJumps, std::unordered_map<std::string, size_t> &labelPositions);
  
  // handle function calls that called for function that was declared afterwards, and thus the index was not known at the time.
  void HandleFunctionCallsWithUndeclaredNames();
  
  VMState &m_state;

  std::string m_path;
  uint32_t m_line;
  std::ifstream m_scriptFile;

  
  std::unordered_map<std::string, ByteCodeCreatorFunction> m_byteCodeCreator;
  std::unordered_map<std::string, size_t> m_staticNameToIndexMapping;
  std::unordered_map<std::string, size_t> m_functionNameToIndexMapping;

  
  // caller name, callee name, index of placeholder bytecode
  std::vector<std::tuple<std::string, std::string, size_t>> m_funcionCallsWithUndeclaredNames;

};