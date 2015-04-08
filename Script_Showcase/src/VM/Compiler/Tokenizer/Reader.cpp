#include "VM/Compiler/Tokenizer/Reader.h"

Reader::Reader() : m_lineNumber(1), m_column(1) {

}

void Reader::Open(const std::string &path) {
  m_file.open(path);
  if (!m_file.is_open()) {
    throw std::runtime_error("Could not open script file '" + path + "'");
  }
}

int Reader::Get() {
  int ch = m_file.get();
  if (ch == EOF) {
    return ch;
  }

  if (ch == '\n') {
    m_column = 1;
    ++m_lineNumber;
  }
  else {
    ++m_column;
  }
  return ch;
}

int Reader::Peek() {
  return m_file.peek();
}

void Reader::SkipWhiteSpace() {
  while (true) {
    auto ch = Peek();
    if (ch == '\n' || ch == '\t' || ch == ' ') {
      Get();
    }
    else {
      return;
    }
  }
}