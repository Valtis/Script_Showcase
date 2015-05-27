
#include "gtest/gtest.h"
#include "VM/Core/VMOperations.h"
#include "VM/Core/VMValue.h"
#include "VM/Core/VMFrame.h"

#include <cmath>
#include <vector>

// Tests for addition


TEST(VMOperations, AddOperationAddsTwoIntegersAndPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(5), VMValue(6) };
  Op::Add(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(11, stack[0].AsInt());
}

TEST(VMOperations, AddOperationAddsIntegerAndFloatAndPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(5), VMValue(6.52f) };
  Op::Add(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(11.52f, stack[0].AsFloat());
}


TEST(VMOperations, AddOperationAddsTwoFloatsAndPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.32f), VMValue(6.52f) };
  Op::Add(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(11.84f, stack[0].AsFloat());
}

TEST(VMOperations, AddOperationAddsDoubleAndIntegerAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.0), VMValue(6) };
  Op::Add(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(11.0, stack[0].AsDouble());
}

TEST(VMOperations, AddOperationAddsDoubleAndFloatAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.0f), VMValue(6.52) };
  Op::Add(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(11.52, stack[0].AsDouble());
}

TEST(VMOperations, AddOperationAddsTwoDoublesAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.32), VMValue(6.52) };
  Op::Add(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(11.84, stack[0].AsDouble());
}

TEST(VMOperations, AddOperationWithInvalidOperandThrows) {
  std::vector<VMValue> stack = { VMValue(5), VMValue('c') };
  EXPECT_THROW(Op::Add(stack), std::runtime_error);
}


TEST(VMOperations, IntegerAddOperationWithTwoIntegersPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(6) };
  Op::AddInteger(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(8, stack[0].AsInt());
}


TEST(VMOperations, IntegerAddOperationThrowsIfOperandIsNotInteger) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(6.2f) };
  EXPECT_THROW(Op::AddInteger(stack), std::runtime_error);

}

// Tests for subtraction

TEST(VMOperations, SubOperationSubtractsTwoIntegersAndPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(5), VMValue(6) };
  Op::Sub(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(-1, stack[0].AsInt());
}

TEST(VMOperations, SubOperationSubtractsIntegerAndFloatAndPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(5), VMValue(6.52f) };
  Op::Sub(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(-1.52f, stack[0].AsFloat());
}


TEST(VMOperations,SubOperationSubtractsTwoFloatsAndPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.32f), VMValue(6.52f) };
  Op::Sub(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(-1.2f, stack[0].AsFloat());
}

TEST(VMOperations, SubOperationSubtractsDoubleAndIntegerAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.0), VMValue(6) };
  Op::Sub(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(-1.0, stack[0].AsDouble());
}

TEST(VMOperations, SubOperationSubtractsDoubleAndFloatAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.0f), VMValue(6.52) };
  Op::Sub(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(-1.52, stack[0].AsDouble());
}

TEST(VMOperations, SubOperationSubtractsTwoDoublesAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.32), VMValue(6.52) };
  Op::Sub(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(-1.2, stack[0].AsDouble());
}

TEST(VMOperations, SubOperationWithInvalidOperandThrows) {
  std::vector<VMValue> stack = { VMValue(5), VMValue(false) };
  EXPECT_THROW(Op::Sub(stack), std::runtime_error);
}


TEST(VMOperations, IntegerSubOperationWithTwoIntegersPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(6) };
  Op::SubInteger(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(-4, stack[0].AsInt());
}


TEST(VMOperations, IntegerSubOperationThrowsIfOperandIsNotInteger) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(6.2) };
  EXPECT_THROW(Op::SubInteger(stack), std::runtime_error);
}

// Tests for multiplication

TEST(VMOperations, MulOperationMultipliesTwoIntegersAndPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(5), VMValue(6) };
  Op::Mul(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(30, stack[0].AsInt());
}

TEST(VMOperations, MulOperationMultipliesIntegerAndFloatAndPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(5), VMValue(6.52f) };
  Op::Mul(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(32.6f, stack[0].AsFloat());
}


TEST(VMOperations, MulOperationMultipliesTwoFloatsAndPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.32f), VMValue(6.52f) };
  Op::Mul(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(34.6864f, stack[0].AsFloat());
}

TEST(VMOperations, MulOperationMultipliesDoubleAndIntegerAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.0), VMValue(6) };
  Op::Mul(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(30.0, stack[0].AsDouble());
}

TEST(VMOperations, MulOperationMultipliesDoubleAndFloatAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.0f), VMValue(6.52) };
  Op::Mul(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(32.6, stack[0].AsDouble());
}

TEST(VMOperations, MulOperationMultipliesTwoDoublesAndPushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(5.32), VMValue(6.52) };
  Op::Mul(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(34.6864, stack[0].AsDouble());
}

TEST(VMOperations, MulOperationWithInvalidOperandThrows) {

  std::vector<VMValue> stack = { VMValue(5) };
  VMValue v;
  v.SetFunction(2);
  stack.push_back(v);
  EXPECT_THROW(Op::Mul(stack), std::runtime_error);
}


TEST(VMOperations, IntegerMulOperationWithTwoIntegersPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(6) };
  Op::MulInteger(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(12, stack[0].AsInt());
}


TEST(VMOperations, IntegerMulOperationThrowsIfOperandIsNotInteger) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(6.2) };
  EXPECT_THROW(Op::MulInteger(stack), std::runtime_error);
}

// Tests for division

TEST(VMOperations, DivisionBetweenTwoIntegersPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(7), VMValue(4) };  
  Op::Div(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(1, stack[0].AsInt());
}

TEST(VMOperations, DivisionBetweenIntegerAndFloatPushesFloatIntoStack) {
  std::vector<VMValue> stack = { VMValue(7), VMValue(4.0f) };
  Op::Div(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(1.75f, stack[0].AsFloat());
}

TEST(VMOperations, DivisionBetweenIntegerAndDoublePushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(7), VMValue(4.0) };
  Op::Div(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(1.75, stack[0].AsDouble());
}

TEST(VMOperations, DivisionBetweenFloatAndDoublePushesDoubleIntoStack) {
  std::vector<VMValue> stack = { VMValue(7.0f), VMValue(4.0) };
  Op::Div(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(1.75, stack[0].AsDouble());
}

TEST(VMOperations, DivisionBetweenTwoFloatsPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(7.0f), VMValue(4.0f) };
  Op::Div(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(1.75f, stack[0].AsFloat());
}

TEST(VMOperations, DivisionBetweenTwoDoublesPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(7.0), VMValue(4.0) };
  Op::Div(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_DOUBLE_EQ(1.75, stack[0].AsDouble());
}

TEST(VMOperations, DivisionByZeroWithIntegersThrows) {
  std::vector<VMValue> stack = { VMValue(7), VMValue(0) };
  EXPECT_THROW(Op::Div(stack), std::runtime_error);
}

TEST(VMOperations, DivisionByZeroWithFloatsResultsInInfinity) {
  std::vector<VMValue> stack = { VMValue(7.0f), VMValue(0.0f) };
  Op::Div(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_TRUE(std::isinf(stack[0].AsFloat()));
}

TEST(VMOperations, DivisionByZeroWithDoublesResultsInInfinity) {
  std::vector<VMValue> stack = { VMValue(7.0), VMValue(0.0) };
  Op::Div(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_TRUE(std::isinf(stack[0].AsDouble()));
}


TEST(VMOperations, DividingZeroByZeroWithIntegersThrows) {
  std::vector<VMValue> stack = { VMValue(0), VMValue(0) };
  EXPECT_THROW(Op::Div(stack), std::runtime_error);
}

TEST(VMOperations, DividingZeroByZeroWithFloatsResultsInNaN) {
  std::vector<VMValue> stack = { VMValue(0.0f), VMValue(0.0f) };
  Op::Div(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_TRUE(std::isnan(stack[0].AsFloat()));
}

TEST(VMOperations, DividingZeroByZeroWithDoublesResultsInNaN) {
  std::vector<VMValue> stack = { VMValue(0.0), VMValue(0.0) };
  Op::Div(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_TRUE(std::isnan(stack[0].AsDouble()));
}

TEST(VMOperations, DividingWithInvalidOperandThrows) {
  std::vector<VMValue> stack = { VMValue(0.0), VMValue(&stack) };
  EXPECT_THROW(Op::Div(stack), std::runtime_error);
}

TEST(VMOperations, IntegerDivOperationWithTwoIntegersPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(6) };
  Op::DivInteger(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(0, stack[0].AsInt());
}

TEST(VMOperations, IntegerDivOperationThrowsIfOperandIsNotInteger) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(6.2) };
  EXPECT_THROW(Op::DivInteger(stack), std::runtime_error);
}

TEST(VMOperations, IntegerDivOperationThrowsIfDivisorIsZero) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(0) };
  EXPECT_THROW(Op::DivInteger(stack), std::runtime_error);
}

// tests for modulo


TEST(VMOperations, ModOperationWithTwoIntegerPushesIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(22), VMValue(6) };
  Op::Mod(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(4, stack[0].AsInt());
}

TEST(VMOperations, ModOperationWithNegativeDividendPushesNegativeIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(-22), VMValue(6) };
  Op::Mod(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(-4, stack[0].AsInt());
}

TEST(VMOperations, ModOperationWithNegativeDivisorAndPositiveDividendPushesPositiveIntegerIntoStack) {
  std::vector<VMValue> stack = { VMValue(22), VMValue(-6) };
  Op::Mod(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(4, stack[0].AsInt());
}

TEST(VMOperations, ModOperationWithZeroDivisorThrows) {
  std::vector<VMValue> stack = { VMValue(22), VMValue(0) };
  EXPECT_THROW(Op::Mod(stack), std::runtime_error);
}


// PUSH & POPS


TEST(VMOperations, PushValuePushesGivenValueIntoStack) {
  std::vector<VMValue> stack;
  Op::PushValue(VMValue{ 24.0f }, stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(24.0f, stack[0].AsFloat());
}

TEST(VMOperations, PushValueThrowsOnStackOverflow) {
  std::vector<VMValue> stack;

  for (auto i = 0; i < STACK_SIZE; ++i) {
    Op::PushValue(VMValue(0), stack);
  }

  EXPECT_THROW(Op::PushValue(VMValue{ 24.0f }, stack), std::runtime_error);
}

TEST(VMOperations, PushIntegerPushesGivenIntegerIntoStack) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(4));
  VMFrame frame(&f);
  frames.push_back(frame);

  Op::PushInteger(stack, frames);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(4, stack[0].AsInt());
}


TEST(VMOperations, PushIntegerThrowsOnStackOverflow) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(4));
  VMFrame frame(&f);
  frames.push_back(frame);

  for (int i = 0; i < STACK_SIZE; ++i) {
    Op::PushValue(VMValue(0), stack);
  }

  EXPECT_THROW(Op::PushInteger(stack, frames), std::runtime_error);
}

TEST(VMOperations, PushFloatPushesGivenFloatIntoStack) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  uint32_t value;
  float float_value = 4.32f;
  memcpy(&value, &float_value, sizeof(value));
  f.AddByteCode(static_cast<ByteCode>(value));
  VMFrame frame(&f);
  frames.push_back(frame);

  Op::PushFloat(stack, frames);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_EQ(4.32f, stack[0].AsFloat());
}


TEST(VMOperations, PushFloatThrowsOnStackOverflow) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  uint32_t value;
  float float_value = 4.32f;
  memcpy(&value, &float_value, sizeof(value));
  f.AddByteCode(static_cast<ByteCode>(value));
  VMFrame frame(&f);
  frames.push_back(frame);

  for (int i = 0; i < STACK_SIZE; ++i) {
    Op::PushValue(VMValue(0), stack);
  }

  EXPECT_THROW(Op::PushFloat(stack, frames), std::runtime_error);
}

TEST(VMOperations, PushDoublePushesGivenDoubleIntoStack) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  uint32_t value[2];
  double double_value = 4.3221;

  memcpy(value, &double_value, sizeof(value));
  
  f.AddByteCode(static_cast<ByteCode>(value[1]));
  f.AddByteCode(static_cast<ByteCode>(value[0]));
  VMFrame frame(&f);
  frames.push_back(frame);

  Op::PushDouble(stack, frames);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::DOUBLE, stack[0].GetType());
  ASSERT_EQ(4.3221, stack[0].AsDouble());
}


TEST(VMOperations, PushDoubleThrowsOnStackOverflow) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  uint32_t value[2];
  double double_value = 4.3221;
  memcpy(value, &double_value, sizeof(value));
  f.AddByteCode(static_cast<ByteCode>(value[1]));
  f.AddByteCode(static_cast<ByteCode>(value[0]));
  VMFrame frame(&f);

  frames.push_back(frame);

  for (int i = 0; i < STACK_SIZE; ++i) {
    Op::PushValue(VMValue(0), stack);
  }

  EXPECT_THROW(Op::PushDouble(stack, frames), std::runtime_error);
}

TEST(VMOperations, PushBooleanPushesTrueIntoStack) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  
  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(true));

  VMFrame frame(&f);
  frames.push_back(frame);

  Op::PushBoolean(stack, frames);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, PushBooleanPushesFalseIntoStack) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;

  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(false));

  VMFrame frame(&f);
  frames.push_back(frame);

  Op::PushBoolean(stack, frames);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, PushBooleanThrowsOnStackOverflow) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(false));
  VMFrame frame(&f);

  frames.push_back(frame);

  for (int i = 0; i < STACK_SIZE; ++i) {
    Op::PushValue(VMValue(0), stack);
  }

  EXPECT_THROW(Op::PushBoolean(stack, frames), std::runtime_error);
}

TEST(VMOperations, PushFunctionPushesGivenFunctionIdIntoStack) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(4u));
  VMFrame frame(&f);
  frames.push_back(frame);

  Op::PushFunction(stack, frames);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FUNCTION, stack[0].GetType());
  ASSERT_EQ(4, stack[0].AsFunction());
}


TEST(VMOperations, PushFunctionThrowsOnStackOverflow) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(4u));
  VMFrame frame(&f);
  frames.push_back(frame);

  for (int i = 0; i < STACK_SIZE; ++i) {
    Op::PushValue(VMValue(0), stack);
  }

  EXPECT_THROW(Op::PushFunction(stack, frames), std::runtime_error);
}