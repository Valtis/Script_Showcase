#pragma once
#include <stdexcept>
#include <string>

// exception for test runner; thrown when assertion fails
class AssertionException : public std::runtime_error {
public:

  AssertionException(const std::string &what, const int line, const std::string &file);

  int line() const;
  std::string file() const;

private:
  int m_line;
  std::string m_file;
};
