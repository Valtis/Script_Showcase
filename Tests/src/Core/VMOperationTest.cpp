
#include "gtest/gtest.h"
#include "VM/Core/VMOperations.h"
#include "VM/Core/VMValue.h"
#include <cmath>
#include <vector>

TEST(VMOperations, AddOperationAddsTwoIntegersAndPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(5), VMValue(6) };
  Op::Add(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::INT);
  ASSERT_EQ(stack[0].AsInt(), 11); 
}

TEST(VMOperations, AddOperationAddsIntegerAndFloatAndPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(5), VMValue(6.52f) };
  Op::Add(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::FLOAT);
  ASSERT_EQ(stack[0].AsFloat(), 11.52f);
}


TEST(VMOperations, AddOperationAddsTwoFloatsAndPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.32f), VMValue(6.52f) };
  Op::Add(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::FLOAT);
  ASSERT_EQ(stack[0].AsFloat(), 11.84f);
}

TEST(VMOperations, AddOperationAddsDoubleAndIntegerAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.0), VMValue(6) };
  Op::Add(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::DOUBLE);
  ASSERT_EQ(stack[0].AsDouble(), 11.0);
}

TEST(VMOperations, AddOperationAddsDoubleAndFloatAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.0f), VMValue(6.52) };
  Op::Add(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::DOUBLE);
  ASSERT_EQ(stack[0].AsDouble(), 11.52);
}

TEST(VMOperations, AddOperationAddsTwoDoublesAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.32), VMValue(6.52) };
  Op::Add(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::DOUBLE);
  ASSERT_EQ(stack[0].AsDouble(), 11.84);
}

TEST(VMOperations, AddOperationWithInvalidOperandThrows) {
  std::vector<VMValue> stack = { VMValue(5), VMValue('c') };
  EXPECT_THROW(Op::Add(stack), std::runtime_error);
}

TEST(VMOperations, DivisionBetweenTwoIntegersPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(7), VMValue(4) };  
  Op::Div(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::INT);
  ASSERT_EQ(stack[0].AsInt(), 1);
}

TEST(VMOperations, DivisionBetweenIntegerAndFloatPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(7), VMValue(4.0f) };
  Op::Div(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::FLOAT);
  ASSERT_EQ(stack[0].AsFloat(), 1.75f);
}

TEST(VMOperations, DivisionBetweenIntegerAndDoublePushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(7), VMValue(4.0) };
  Op::Div(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::DOUBLE);
  ASSERT_EQ(stack[0].AsDouble(), 1.75);
}

TEST(VMOperations, DivisionBetweenFloatAndDoublePushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(7.0f), VMValue(4.0) };
  Op::Div(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::DOUBLE);
  ASSERT_EQ(stack[0].AsDouble(), 1.75);
}

TEST(VMOperations, DivisionBetweenTwoFloatsPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(7.0f), VMValue(4.0f) };
  Op::Div(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::FLOAT);
  ASSERT_EQ(stack[0].AsFloat(), 1.75f);
}

TEST(VMOperations, DivisionBetweenTwoDoublesPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(7.0), VMValue(4.0) };
  Op::Div(stack);
  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::DOUBLE);
  ASSERT_EQ(stack[0].AsDouble(), 1.75);
}

TEST(VMOperations, DivisionByZeroWithIntegersThrows) {
  std::vector<VMValue> stack = { VMValue(7), VMValue(0) };
  EXPECT_THROW(Op::Div(stack), std::runtime_error);
}

TEST(VMOperations, DivisionByZeroWithFloatsResultsInInfinity) {
  std::vector<VMValue> stack = { VMValue(7.0f), VMValue(0.0f) };
  Op::Div(stack);

  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::FLOAT);
  ASSERT_TRUE(std::isinf(stack[0].AsFloat()));
}

TEST(VMOperations, DivisionByZeroWithDoublesResultsInInfinity) {
  std::vector<VMValue> stack = { VMValue(7.0), VMValue(0.0) };
  Op::Div(stack);

  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::DOUBLE);
  ASSERT_TRUE(std::isinf(stack[0].AsDouble()));
}


TEST(VMOperations, DividingZeroByZeroWithIntegersThrows) {
  std::vector<VMValue> stack = { VMValue(0), VMValue(0) };
  EXPECT_THROW(Op::Div(stack), std::runtime_error);
}

TEST(VMOperations, DividingZeroByZeroWithFloatsResultsInNaN) {
  std::vector<VMValue> stack = { VMValue(0.0f), VMValue(0.0f) };
  Op::Div(stack);

  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::FLOAT);
  ASSERT_TRUE(std::isnan(stack[0].AsFloat()));
}

TEST(VMOperations, DividingZeroByZeroWithDoublesResultsInNaN) {
  std::vector<VMValue> stack = { VMValue(0.0), VMValue(0.0) };
  Op::Div(stack);

  ASSERT_EQ(stack.size(), 1);
  ASSERT_EQ(stack[0].GetType(), ValueType::DOUBLE);
  ASSERT_TRUE(std::isnan(stack[0].AsDouble()));
}

TEST(VMOperations, DividingWithInvalidOperandThrows) {
  std::vector<VMValue> stack = { VMValue(0.0), VMValue(&stack) };
  EXPECT_THROW(Op::Div(stack), std::runtime_error);
}
