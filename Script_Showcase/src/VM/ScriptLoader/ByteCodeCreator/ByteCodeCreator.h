#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <tuple>
class VMFunction;

struct ByteCodeCreatorArguments {
  VMFunction &function; 
  std::unordered_map<std::string, size_t> &staticNameToIndexMapping;
  std::unordered_map<std::string, size_t> &localToIndexMapping;
  std::unordered_map<std::string, size_t> &functionNameToIndexMapping;
  std::vector<std::tuple<std::string, std::string, size_t>> &funcionCallsWithUndeclaredNames;
  std::unordered_map<std::string, size_t> &labelPositions;
  std::unordered_map<std::string, size_t> &unhandledJumps; 
  std::vector<std::string> &tokens;
};

typedef void(*ByteCodeCreatorFunction)(ByteCodeCreatorArguments args);

namespace ByteCodeCreator {
    
  std::unordered_map<std::string, ByteCodeCreatorFunction> CreateByteCodeCreators();  

  int32_t ConvertToInteger(const std::string &str);
  void ExpectTokenCount(const std::vector<std::string> &tokens, const int count);
  void ExpectToken(const std::string &token, const std::string &expected);
  void ExpectNameExists(const std::unordered_map<std::string, size_t> &staticNameToIndexMappings, const std::string &name);
}