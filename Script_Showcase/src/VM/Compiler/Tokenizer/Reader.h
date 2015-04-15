#pragma once

#include <fstream>
#include <string>
class Reader {
public:
  Reader();
  void Open(const std::string &path);
  void SkipWhiteSpace();
  int Peek();
  int Get();

  int GetLine() { return m_lineNumber; }
  int GetColumn() { return m_column; }
private:
  std::ifstream m_file;
  int m_lineNumber;
  int m_column;
};