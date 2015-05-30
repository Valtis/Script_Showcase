
#include "gtest/gtest.h"
#include "VM/Exceptions/Exceptions.h"
#include "VM/FFI/NativeBinding.h"

float void_free_function_value;
float void_member_function_value;
float void_static_member_function_value;

void void_free_function(int a, float b) {
  void_free_function_value = a + 2*b;
}

double non_void_free_function(int a, float b) {
  return static_cast<double>(a + 2*b);
}


class TestClass {
public:
  void void_member_function(int a, float b) {
    void_member_function_value = a + 2*b + value*3;
  }

  double non_void_member_function(int a, float b) {
    return a + 2 *b + value*3;
  }

  static void void_static_member_function(int a, float b) {
    void_static_member_function_value = a + 2*b;
  }

  static double non_void_static_member_function(int a, float b) {
    return a + 2*b;
  }

  int value;
  
};

TEST(NativeBinding, VoidFreeFunctionIsCreatedAndIsCallable) {
  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 7.0f } };
  void_free_function_value = 0.0f;
  auto binding = CreateBinding(&void_free_function);
  binding(stack);
  ASSERT_EQ(0, stack.size());
  ASSERT_FLOAT_EQ(19.0f, void_free_function_value);
}

TEST(NativeBinding, VoidFreeFunctionThrowsOnTypeError) {
  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 'c' } };
  void_free_function_value = 0.0f;
  auto binding = CreateBinding(&void_free_function);
  ASSERT_THROW(binding(stack), TypeError);
}

TEST(NativeBinding, NonVoidFreeFunctionIsCreatedAndIsCallable) {
  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 7.0f } };
  auto binding = CreateBinding(&non_void_free_function);
  binding(stack);
 
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(19.0, stack[0].AsDouble());
}

TEST(NativeBinding, NonVoidFreeFunctionThrowsOnTypeError) {
  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 'c' } };
  auto binding = CreateBinding(&non_void_free_function);
  ASSERT_THROW(binding(stack), TypeError);
}

TEST(NativeBinding, VoidMemberFunctionIsCreatedAndIsCallable) {
  TestClass instance;
  instance.value = 7;
  std::vector<VMValue> stack = { VMValue{ &instance }, VMValue{ 5 }, VMValue{ 7.0f } };
  void_member_function_value = 0.0f;
  auto binding = CreateBinding(&TestClass::void_member_function);
  binding(stack);
  ASSERT_EQ(0, stack.size());
  ASSERT_FLOAT_EQ(40.0f, void_member_function_value);
}

TEST(NativeBinding, VoidMemberFunctionThrowsOnTypeError) {
  TestClass instance;
  instance.value = 7;
  std::vector<VMValue> stack = { VMValue{ &instance }, VMValue{ 5 }, VMValue{ 'c' } };
  void_member_function_value = 0.0f;
  auto binding = CreateBinding(&TestClass::void_member_function);
  ASSERT_THROW(binding(stack), TypeError);
}

TEST(NativeBinding, NonVoidMemberFunctionIsCreatedAndIsCallable) {
  TestClass instance;
  instance.value = 7;
  std::vector<VMValue> stack = { VMValue{ &instance }, VMValue{ 5 }, VMValue{ 7.0f } };
  auto binding = CreateBinding(&TestClass::non_void_member_function);
  binding(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(40.0, stack[0].AsDouble());
}

TEST(NativeBinding, NonVoidMemberFunctionThrowsOnTypeError) {
  TestClass instance;
  instance.value = 7;
  std::vector<VMValue> stack = { VMValue{ &instance }, VMValue{ 5 }, VMValue{ 'c' } };
  auto binding = CreateBinding(&TestClass::non_void_member_function);
  ASSERT_THROW(binding(stack), TypeError);
}

TEST(NativeBinding, VoidStaticMemberFunctionIsCreatedAndIsCallable) {

  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 7.0f } };
  void_static_member_function_value = 0.0f;
  auto binding = CreateBinding(&TestClass::void_static_member_function);
  binding(stack);
  ASSERT_EQ(0, stack.size());
  ASSERT_FLOAT_EQ(19.0f, void_static_member_function_value);
}

TEST(NativeBinding, VoidStaticMemberFunctionThrowsOnTypeError) {

  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 'c' } };
  void_static_member_function_value = 0.0f;
  auto binding = CreateBinding(&TestClass::void_static_member_function);
  ASSERT_THROW(binding(stack), TypeError);
}

TEST(NativeBinding, NonVoidStaticMemberFunctionIsCreatedAndIsCallable) {

  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 7.0f } };
  auto binding = CreateBinding(&TestClass::non_void_static_member_function);
  binding(stack); 

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(19, stack[0].AsDouble());
}

TEST(NativeBinding, NonVoidStaticMemberFunctionThrowsOnTypeError) {

  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 'c' } };
  auto binding = CreateBinding(&TestClass::non_void_member_function);
  ASSERT_THROW(binding(stack), TypeError);
}

