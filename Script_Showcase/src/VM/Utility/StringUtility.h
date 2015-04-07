#pragma once
#include <vector>
#include <string>

namespace Utility
{
  std::vector<std::string> Tokenize(std::string text, std::string delimiters);
  std::vector<std::string> HandleControlCharacters(std::string line);
  std::vector<std::string> WrapLines(std::vector<std::string> lines, double lineWidth, double characterWidth);
  std::string Trim(const std::string &str);
}