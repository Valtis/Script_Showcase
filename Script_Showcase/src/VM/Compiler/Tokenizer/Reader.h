#pragma once

#include <fstream>
#include <string>

/*
  Reader class which is used by the tokenizer. Abstracts Peek/Get-operations and tracks current line and column numbers
*/
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