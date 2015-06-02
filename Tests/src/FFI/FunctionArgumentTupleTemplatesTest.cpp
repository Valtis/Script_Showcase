
#include "gtest/gtest.h"
#include "VM/FFI/FunctionArgumentTupleTemplates.h"
#include "VM/Exceptions/Exceptions.h"

TEST(FunctionArgumentTupleTemplates, ConstructParameterTupleConstructsTupleWithCorrectValues) {
  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 42.3f }, VMValue{ true }, VMValue{ 5.0 } };
  auto tuple = ConstructParameterTuple<float, bool, double>(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(5, stack[0].AsInt());

  ASSERT_FLOAT_EQ(42.3f, std::get<0>(tuple));
  ASSERT_TRUE(std::get<1>(tuple));
  ASSERT_DOUBLE_EQ(5.0, std::get<2>(tuple));
}

TEST(FunctionArgumentTupleTemplates, ConstructParameterTupleWorksWithEmptyArgumentTypeList) {
  std::vector<VMValue> stack = { VMValue{ 5 } };
  auto tuple = ConstructParameterTuple<>(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(5, stack[0].AsInt());
}

TEST(FunctionArgumentTupleTemplates, ConstructParameterTupleThrowsOnTypeError) {
  std::vector<VMValue> stack = { VMValue{ 5 }, VMValue{ 42.3f }, VMValue{ 32 }, VMValue{ 5.0 } };
  // additional parenthesis required with at least vs 2013. Might be an issue with gtest and templates
  EXPECT_THROW((ConstructParameterTuple<float, bool, double>(stack)), TypeError);
}
