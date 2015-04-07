#pragma once

#include "test_runner.h"
#include "exceptions.h"



// helper macros

#define RUN_TESTS() test_runner.run_tests()
#define TEST(test_name) class TEST_CLASS_ ## test_name  {                \
  public:                                                                         \
    TEST_CLASS_ ## test_name () {  \
      test_runner.register_test(#test_name, [=]() { test(); });                   \
    }                                                                             \
                                                                                  \
  void test();                                                                    \
} test_instance ## test_name;                                             \
 void TEST_CLASS_ ## test_name :: test()                                      \

// Test macros
#define ASSERT_EQUALS(expected, actual) assert_equality((expected), (actual), __LINE__, __FILE__)
#define ASSERT_TRUE(value) assert_true((value), __LINE__, __FILE__)
#define ASSERT_FALSE(value) assert_false((value), __LINE__, __FILE__)


// have to be a macro instead of function as the x must first be evaluated inside try block
#define ASSERT_THROWS(x, exception_type) {  auto line = __LINE__; auto file = __FILE__; bool thrown = false; try { \
  (x); \
  } catch (const exception_type &ex) { \
    thrown = true; \
  } \
  catch (const std::exception &ex) { \
    throw AssertionException(std::string("Unexpected exception with message: ") + ex.what(), line, file); \
  } \
  catch (...) {\
    throw AssertionException("Unexpected exception of unknown type caught ", line, file); \
  }\
  if (!thrown) { \
    throw AssertionException(std::string("No exception of type ") + #exception_type + " was thrown", line, file); \
  }}

// due to templates, these must be defined in the header

template <typename T>
void assert_equality(T expected, T actual, int line, std::string file) {
  if (expected != actual) {
    throw AssertionException("Equality failure: Expected '" + std::to_string(expected)
        + "' but was '" + std::to_string(actual) + "'",
        line, file);
  }
}

void assert_true(bool value, int line, std::string file);
void assert_false(bool value, int line, std::string file);
