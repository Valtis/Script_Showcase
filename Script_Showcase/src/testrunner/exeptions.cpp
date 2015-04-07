#include "exceptions.h"


AssertionException::AssertionException(const std::string &msg, const int line, const std::string &file)
  : std::runtime_error(msg), m_line(line), m_file(file) {
}


int AssertionException::line() const {
   return m_line;
}

std::string AssertionException::file() const {
  return m_file;
}
