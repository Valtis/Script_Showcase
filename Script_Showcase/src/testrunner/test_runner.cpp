#include "test_runner.h"
#include <iostream>
#include "exceptions.h"
#include "leakdetection/memory_instrumentation.h"

test_runner_class test_runner;

test_runner_class::test_runner_class() : m_passed_tests(0), m_failed_tests(0) {

}

void test_runner_class::register_test(const std::string &name, std::function<void()> test) {
  m_test_cases.push_back({ name, test });
}


void test_runner_class::run_tests() {
  std::cout << "\nRunning tests...\n\n";

  for (const test_case &test : m_test_cases) {
    handle_test_case(test);
  }

  std::cout << "\n" << m_passed_tests << " tests passed, " << m_failed_tests << " failed.\n";
}


void test_runner_class::handle_test_case(const test_case &test) {
  bool passing = false;

  auto allocations = get_allocations_list();
  allocations->clear(); // clear allocation tracking so that any previous errors won't cause false positives

  try {
    test.test_function();
    passing = true;
  } catch (const AssertionException &ex) {
    std::cout << "Assertion failure at test " << test.test_name << ":\n   " << ex.what() << "\n";
    std::cout << "Failed at line " << ex.line() << " at file " << ex.file() << "\n";
  } catch (const std::exception &ex) {
    std::cout << "Unexpected exception caught: " << ex.what() << "\n";
  } catch (...) {
    std::cout << "Unexpected exception with unknown type caught\n";
  }

  if (allocations->has_errors()) {
    std::cout << "Memory leak(s) detected in test case " << test.test_name << "\n";
    allocations->print_errors();
    passing = false;
  }

  if (passing) {
    std::cout << "Test " << test.test_name << " passed!\n";
    ++m_passed_tests;
  } else {
    std::cout << "Test " << test.test_name << " failed!\n";
    ++m_failed_tests;
  }

  std::cout << "\n";
}
