#include "VM/Core/VMState.h"
#include "VM/Core/VMFunction.h"
#include "VM/Memory/MemoryManager.h"
#include "VM/ScriptLoader/ScriptLoader.h"
#include "VM/ScriptLoader/ByteCodeCreator/ByteCodeCreator.h"
#include "VM/ScriptLoader/Tokens.h"
#include "VM/Utility/StringUtility.h"

ScriptLoader::ScriptLoader(VMState &state, std::string path) : m_state(state), m_path(path), m_line(0) {
  m_byteCodeCreator = ByteCodeCreator::CreateByteCodeCreators();
}

void ScriptLoader::Load() {
  m_scriptFile.open(m_path);
  if (!m_scriptFile.is_open()) {
    throw std::runtime_error("Could not open script file '" + m_path + "'");
  }


  try {
    LoadStatics();
    LoadFunctions();
  }
  catch (std::exception &ex) {
    std::string err = ex.what();
    throw std::runtime_error("An error occurred while processing script file '" + m_path + "' at line " + std:: to_string(m_line) + ": " + err);
  }

}

void ScriptLoader::LoadStatics() {
  std::string line;
  while (GetLine(line)) {
    if (!line.empty()) {
      break;
    }
  }

  ByteCodeCreator::ExpectToken(line, STATICS_TOKEN);

  while (GetLine(line)) {
    auto tokens = Utility::Tokenize(line, " ");
    if (tokens[0] == END_STATICS_TOKEN) {
      return;
    }

    AddStaticValue(tokens);

  }

}


void ScriptLoader::AddStaticValue(const std::vector<std::string> &tokens) {
  ByteCodeCreator::ExpectTokenCount(tokens, 4);
  ByteCodeCreator::ExpectToken(tokens[2], AS_TOKEN);
  VMValue obj;
  
  auto name = tokens[3];
  auto value = tokens[1];
  
  if (m_staticNameToIndexMapping.find(name) != m_staticNameToIndexMapping.end()) {
    throw std::runtime_error("Multiple declarations of static value " + name);
  }
  
  if (tokens[0] == STRING_TOKEN) {
    obj = MemMgrInstance().AllocateArray(ValueType::CHAR, value.length());
    MemMgrInstance().WriteToArrayIndex(obj, &value[0], 0, value.length());

  } else if (tokens[0] == INTEGER_TOKEN) {
    obj = VMValue{ ByteCodeCreator::ConvertToInteger(value) };
    
  }
  else if (tokens[0] == NATIVE_POINTER_TOKEN) {
    obj.set_native_pointer(nullptr);
  }
  else {
    throw std::runtime_error("Unexpected token " + tokens[0] + ". Expected type declaration");
  }

  auto index = m_state.AddStaticObject(obj);
  m_staticNameToIndexMapping[name] = index;
}


void ScriptLoader::LoadFunctions() {
  std::string line;
  while (GetLine(line)) {
    if (line.empty()) {
      continue;
    }

    auto tokens = Utility::Tokenize(line, " ");
    ByteCodeCreator::ExpectToken(tokens[0], FUNCTION_TOKEN);
    ByteCodeCreator::ExpectTokenCount(tokens, 2);

    LoadFunction(tokens[1]);
  }

  HandleFunctionCallsWithUndeclaredNames();
}

void ScriptLoader::LoadFunction(const std::string &name) {
  VMFunction function;
  function.SetName(name);
  
  auto locals = LoadLocals();
  
  std::unordered_map<std::string, size_t> labelPositions; // targets for jump instructions

  // contains the jumps where label was not yet processed (jumping forward)
  // name - placeholder bytecode location
  std::unordered_map<std::string, size_t> unhandledJumps; 

  std::string line;
  while (GetLine(line)) {
    if (line.empty()) {
      continue;
    }
    
    if (line == END_FUNCTION_TOKEN) {
      break;
    }

    auto tokens = Utility::Tokenize(line, " ");
    try {
      m_byteCodeCreator.at(tokens[0])({ function, m_staticNameToIndexMapping, locals, m_functionNameToIndexMapping, m_funcionCallsWithUndeclaredNames, labelPositions, unhandledJumps, tokens });
    } catch (const std::out_of_range &ex) {
      throw std::runtime_error("Unexpected token '" + tokens[0] + "'. Expected a command");
    }
  }

  HandleJumpsWithUndeclaredTargets(function, unhandledJumps, labelPositions);


  function.SetLocalCount(locals.size());
  function.AddByteCode(ByteCode::RETURN);
  m_functionNameToIndexMapping[name] = m_state.AddFunction(function);
}


void ScriptLoader::HandleJumpsWithUndeclaredTargets(VMFunction &function, std::unordered_map<std::string, size_t> &unhandledJumps, std::unordered_map<std::string, size_t> &labelPositions)
{
  for (auto pair : unhandledJumps) {
    if (labelPositions.find(pair.first) == labelPositions.end()) {
      throw std::runtime_error("Could not find label '" + pair.first + "'");
    }
    function.ChangeByteCode(pair.second, static_cast<ByteCode>(labelPositions[pair.first]));
  }
}

void ScriptLoader::HandleFunctionCallsWithUndeclaredNames() {
  for (auto &call : m_funcionCallsWithUndeclaredNames) {
    auto it = m_functionNameToIndexMapping.find(std::get<1>(call));
    if (it == m_functionNameToIndexMapping.end()) {
      throw std::runtime_error("Could not find function name " + std::get<1>(call));
    }
    // GetFunction returns const pointer as this is meant for runtime usage where functions should not be modified
    // however, in this context, we do need to modify the placeholder bytecode, so we need to cast the constness away
    auto function = const_cast<VMFunction *>(m_state.GetFunction(std::get<0>(call)));
    function->ChangeByteCode(std::get<2>(call), static_cast<ByteCode>(it->second));
  }
}

std::unordered_map<std::string, size_t> ScriptLoader::LoadLocals()
{
  
  std::string line;
  while (GetLine(line)) {
    if (!line.empty()) {
      break;
    }
  }
  
  ByteCodeCreator::ExpectToken(line, LOCALS_TOKEN);
  std::unordered_map<std::string, uint32_t> localNameToIndexMappings;
  size_t localPos = 0;
  while (GetLine(line)) {
    if (line.empty()) {
      continue;
    }
    if (line == END_LOCALS_TOKEN) {
      break;
    }

    auto tokens = Utility::Tokenize(line, " ");
    ByteCodeCreator::ExpectTokenCount(tokens, 1);

    if (localNameToIndexMappings.find(tokens[0]) != localNameToIndexMappings.end()) {
      throw std::runtime_error("Local variable " + tokens[0] + " has already been defined");
    }
    localNameToIndexMappings[tokens[0]] = localPos++;
  }
  return localNameToIndexMappings;
}



bool ScriptLoader::GetLine(std::string &line) {
  
  if (!std::getline(m_scriptFile, line)) {
    return false;
  }
  ++m_line;
  line = line.substr(0, line.find("#")); // strip comments, if any
  line = Utility::Trim(line);

  return true;
}

