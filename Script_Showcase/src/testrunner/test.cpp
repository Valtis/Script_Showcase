#include "test.h"

void assert_true(bool value, int line, std::string file) {
  if (value != true) {
    throw AssertionException("Expression is false",
        line, file);
  }
}


void assert_false(bool value, int line, std::string file) {
  if (value != false) {
    throw AssertionException("Expression is true",
        line, file);
  }
}
