#pragma once
#include <string>
#include <functional>
#include <vector>

/*
Test runner class. Runs registered test cases and reports whether the test passed or not
*/
class test_runner_class {
public:

  test_runner_class();

  // registers tests for running. Test must be in form of void function without parameters
  void register_test(const std::string &name, std::function<void()> test);

  // runs registered tests
  void run_tests();

private:

  // helper struct for test cases
  struct test_case {
    std::string test_name;
    std::function<void()> test_function;
  };

  // handles individual test case when running tests
  void handle_test_case(const test_case &test);

  std::vector<test_case> m_test_cases;
  int m_passed_tests;
  int m_failed_tests;
};

extern test_runner_class test_runner;
