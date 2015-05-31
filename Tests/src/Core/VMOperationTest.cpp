
#include "gtest/gtest.h"
#include "VM/Core/VMOperations.h"
#include "VM/Core/VMValue.h"
#include "VM/Core/VMFrame.h"
#include "VM/Core/VMState.h"
#include "VM/FFI/NativeBinding.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/Memory/MemoryManager.h"
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
  EXPECT_THROW(Op::Add(stack), InvalidConversionError);
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
  EXPECT_THROW(Op::AddInteger(stack), TypeError);

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
  EXPECT_THROW(Op::Sub(stack), InvalidConversionError);
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
  EXPECT_THROW(Op::SubInteger(stack), TypeError);
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
  EXPECT_THROW(Op::Mul(stack), InvalidConversionError);
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
  EXPECT_THROW(Op::MulInteger(stack), TypeError);
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
  EXPECT_THROW(Op::Div(stack), DivisionByZeroError);
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
  EXPECT_THROW(Op::Div(stack), DivisionByZeroError);
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
  EXPECT_THROW(Op::Div(stack), InvalidConversionError);
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
  EXPECT_THROW(Op::DivInteger(stack), TypeError);
}

TEST(VMOperations, IntegerDivOperationThrowsIfDivisorIsZero) {
  std::vector<VMValue> stack = { VMValue(2), VMValue(0) };
  EXPECT_THROW(Op::DivInteger(stack), DivisionByZeroError);
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
  EXPECT_THROW(Op::Mod(stack), DivisionByZeroError);
}

// tests for not

TEST(VMOperations, NotOnTruePushesFalseIntoStack) {
  std::vector<VMValue> stack = { VMValue(true) };
  Op::Not(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, NotOnFalsePushesTrueIntoStack) {
  std::vector<VMValue> stack = { VMValue(false) };
  Op::Not(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, NotThrowsIfOperandIsNonBoolean) {
  std::vector<VMValue> stack = { VMValue(4) };
  EXPECT_THROW(Op::Not(stack), TypeError);
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

  EXPECT_THROW(Op::PushValue(VMValue{ 24.0f }, stack), ValueStackOverFlowError);
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

  EXPECT_THROW(Op::PushInteger(stack, frames), ValueStackOverFlowError);
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

  EXPECT_THROW(Op::PushFloat(stack, frames), ValueStackOverFlowError);
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

  EXPECT_THROW(Op::PushDouble(stack, frames), ValueStackOverFlowError);
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

  EXPECT_THROW(Op::PushBoolean(stack, frames), ValueStackOverFlowError);
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

  EXPECT_THROW(Op::PushFunction(stack, frames), ValueStackOverFlowError);
}

TEST(VMOperations, PopReturnsTopmostValueFromStack) {
  std::vector<VMValue> stack = { VMValue(5.0), VMValue('a') };
  EXPECT_EQ('a', Op::PopValue(stack).AsChar());
}

TEST(VMOperations, PopThrowsOnEmptyStack) {
  std::vector<VMValue> stack;
  EXPECT_THROW(Op::PopValue(stack).AsChar(), ValueStackUnderFlowError);
}


// loads & stores


TEST(VMOperations, StoreLocalStoresValueIntoFrame) {
  std::vector<VMValue> stack = { VMValue(123.45)};
  std::vector<VMFrame> frames;
  VMFunction f;
  f.SetLocalCount(3);
  f.AddByteCode(static_cast<ByteCode>(2));
  VMFrame frame(&f);
  frames.push_back(frame);

  Op::StoreLocal(stack, frames);
  ASSERT_EQ(ValueType::DOUBLE, frames[0].GetLocalVariable(2).GetType());
  ASSERT_DOUBLE_EQ(123.45, frames[0].GetLocalVariable(2).AsDouble());
}

TEST(VMOperations, LoadLocalLoadValueFromFrame) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  VMFunction f;
  f.SetLocalCount(3);
  f.AddByteCode(static_cast<ByteCode>(2));
  VMFrame frame(&f);
  frames.push_back(frame);
  frames[0].SetLocalVariable(2, VMValue('y'));
    
  Op::LoadLocal(stack, frames);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::CHAR, stack[0].GetType());
  ASSERT_EQ('y', stack[0].AsChar());
}

TEST(VMOperations, StoreStaticStoresValueIntoVMState) {
  std::vector<VMValue> stack = { VMValue(123.45) };
  std::vector<VMFrame> frames;  
  VMState state;
  auto index = state.AddStaticObject(VMValue{});
  
  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(index));
  VMFrame frame(&f);
  frames.push_back(frame);



  Op::StoreStaticObject(state, stack, frames);
  ASSERT_EQ(ValueType::DOUBLE, state.GetStaticObject(index).GetType());
  ASSERT_DOUBLE_EQ(123.45, state.GetStaticObject(index).AsDouble());
}

TEST(VMOperations, LoadStaticLoadsValueFromVMState) {
  std::vector<VMValue> stack;
  std::vector<VMFrame> frames;
  
  VMState state;
  auto index = state.AddStaticObject(VMValue{ 'y' });

  VMFunction f;  
  f.AddByteCode(static_cast<ByteCode>(index));

  VMFrame frame(&f);
  frames.push_back(frame);
  
  Op::LoadStaticObject(state, stack, frames);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::CHAR, stack[0].GetType());
  ASSERT_EQ('y', stack[0].AsChar());
}

TEST(VMOperations, StoreArrayIndexStoresValueIntoArray) {
  std::vector<VMValue> stack = { VMValue{}, VMValue{ 4 }, VMValue{ false } };
  auto ptr = stack[0] = MemMgrInstance().AllocateArray(ValueType::BOOL, 5);


  Op::StoreArrayIndex(stack);
  
  bool value;
  MemMgrInstance().ReadFromArrayIndex(ptr, &value, 4, 1);
  ASSERT_EQ(false, value);
}

TEST(VMOperations, StoreArrayIndexThrowsIfArrayAndVMValueTypesDoNotMatch) {
  std::vector<VMValue> stack = { VMValue{}, VMValue{ 4 }, VMValue{ false } };
  auto ptr = stack[0] = MemMgrInstance().AllocateArray(ValueType::INT, 5);

  EXPECT_THROW(Op::StoreArrayIndex(stack), TypeError);
}

TEST(VMOperations, StoreArrayIndexThrowsIfIndexIsOutOfBounds) {
  std::vector<VMValue> stack = { VMValue{}, VMValue{ 20 }, VMValue{ false } };
  auto ptr = stack[0] = MemMgrInstance().AllocateArray(ValueType::BOOL, 5);

  EXPECT_THROW(Op::StoreArrayIndex(stack), ArrayIndexOutOfBoundsError);
}

TEST(VMOperations, StoreArrayIndexThrowsIfNoPointerIsPresent) {
  std::vector<VMValue> stack = { VMValue{}, VMValue{ 0 }, VMValue{ false } };
  EXPECT_THROW(Op::StoreArrayIndex(stack), TypeError);
}

TEST(VMOperations, LoadArrayLoadsFromValueFromArrayIndex) {
  std::vector<VMValue> stack = { VMValue{}, VMValue{ 4 } };
  auto ptr = stack[0] = MemMgrInstance().AllocateArray(ValueType::FLOAT, 5);
  auto value = 421.1f;
  MemMgrInstance().WriteToArrayIndex(ptr, &value, 4, 1);
    Op::LoadArrayIndex(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::FLOAT, stack[0].GetType());
  ASSERT_FLOAT_EQ(421.1f, stack[0].AsFloat());
}

TEST(VMOperations, LoadArrayThrowsIfIndexIsOutOfBounds) {
  std::vector<VMValue> stack = { VMValue{}, VMValue{ 20 } };
  auto ptr = stack[0] = MemMgrInstance().AllocateArray(ValueType::FLOAT, 5);
  EXPECT_THROW(Op::LoadArrayIndex(stack), ArrayIndexOutOfBoundsError);
}

TEST(VMOperations, LoadArrayThrowsIfNoPointerIsPresent) {
  std::vector<VMValue> stack = { VMValue{}, VMValue{ 0 } };
  EXPECT_THROW(Op::LoadArrayIndex(stack), TypeError);
}


TEST(VMOperations, ArrayLengthPushesArrayLengthIntoStack) {
  std::vector<VMValue> stack = { VMValue{} };
  stack[0] = MemMgrInstance().AllocateArray(ValueType::FLOAT, 5);
  Op::ArrayLength(stack);
  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(5, stack[0].AsInt());
}

TEST(VMOperations, ArrayLengthThrowsIfOperandIsNotAPointerToArray) {
  std::vector<VMValue> stack = { VMValue{} };
  EXPECT_THROW(Op::ArrayLength(stack), TypeError);
}


// Comparisons

TEST(VMOperations, IsGreaterPushesTrueWithIntegersIntoStackIfValueIsGreater) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(3) };
  Op::IsGreater(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterPushesFalseWithIntegersIntoStackIfValueIsEqual) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(4) };
  Op::IsGreater(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterPushesFalseWithIntegersIntoStackIfValueIsLesser) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(5) };
  Op::IsGreater(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterPushesTrueIntoStackWhenValueIsGreaterAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.2), VMValue(4) };
  Op::IsGreater(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterPushesFalseIntoStackWhenValueIsEqualAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.0f) };
  Op::IsGreater(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterPushesFalseIntoStackWhenValueIsLesserAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.1f) };
  Op::IsGreater(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterOrEqualPushesTrueWithIntegersIntoStackIfValueIsGreater) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(3) };
  Op::IsGreaterOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterOrEqPushesTrueWithIntegersIntoStackIfValueIsEqual) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(4) };
  Op::IsGreaterOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterOrEqPushesFalseWithIntegersIntoStackIfValueIsLesser) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(5) };
  Op::IsGreaterOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterOrEqPushesTrueIntoStackWhenValueIsGreaterAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.2), VMValue(4) };
  Op::IsGreaterOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterOrEqPushesTrueIntoStackWhenValueIsEqualAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.0f) };
  Op::IsGreaterOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsGreaterOrEqPushesFalseIntoStackWhenValueIsLesserAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.1f) };
  Op::IsGreaterOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsEqualPushesFalseWithIntegersIntoStackIfValueIsGreater) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(3) };
  Op::IsEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsEqualPushesTrueWithIntegersIntoStackIfValueIsEqual) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(4) };
  Op::IsEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsEqualPushesFalseWithIntegersIntoStackIfValueIsLesser) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(5) };
  Op::IsEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsEqualPushesFalseIntoStackWhenValueIsGreaterAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.2), VMValue(4) };
  Op::IsEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsEqualPushesTrueIntoStackWhenValueIsEqualAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.0f) };
  Op::IsEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsEqualPushesFalseIntoStackWhenValueIsLesserAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.1f) };
  Op::IsEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsLessOrEqualPushesFalseWithIntegersIntoStackIfValueIsGreater) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(3) };
  Op::IsLessOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsLessOrEqualPushesTrueWithIntegersIntoStackIfValueIsEqual) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(4) };
  Op::IsLessOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsLessOrEqualPushesTrueWithIntegersIntoStackIfValueIsLesser) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(5) };
  Op::IsLessOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsLessOrEqualPushesFalseIntoStackWhenValueIsGreaterAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.2), VMValue(4) };
  Op::IsLessOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsLessOrEqualPushesTrueIntoStackWhenValueIsEqualAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.0f) };
  Op::IsLessOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsLessOrEqualPushesTrueIntoStackWhenValueIsLesserAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.1f) };
  Op::IsLessOrEq(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsLessPushesFalseWithIntegersIntoStackIfValueIsGreater) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(3) };
  Op::IsLess(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsLessPushesFalseWithIntegersIntoStackIfValueIsEqual) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(4) };
  Op::IsLess(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsLessPushesTrueWithIntegersIntoStackIfValueIsLesser) {
  std::vector<VMValue> stack = { VMValue(4), VMValue(5) };
  Op::IsLess(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

TEST(VMOperations, IsLessPushesFalseIntoStackWhenValueIsGreaterAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.2), VMValue(4) };
  Op::IsLess(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsLessPushesFalseIntoStackWhenValueIsEqualAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.0f) };
  Op::IsLess(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(false, stack[0].AsBool());
}

TEST(VMOperations, IsLessPushesTrueIntoStackWhenValueIsLesserAndRequiresTypeConversion) {
  std::vector<VMValue> stack = { VMValue(4.0), VMValue(4.1f) };
  Op::IsLess(stack);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::BOOL, stack[0].GetType());
  ASSERT_EQ(true, stack[0].AsBool());
}

// Jumps

TEST(VMOperations, UnconditionalJumpModifiesProgramCounter) {
  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(5));
  VMFrame frame(&f);
  std::vector<VMFrame> frames = { frame };
  Op::Jump(frames);
   
  ASSERT_EQ(1, frames.size());
  ASSERT_EQ(5, frames[0].GetProgramCounter());
}


TEST(VMOperations,JumpIfTrueModifiesProgramCounterIfTrueIsInStack) {
  std::vector<VMValue> stack = { VMValue{ true } };

  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(5));
  VMFrame frame(&f);
  std::vector<VMFrame> frames = { frame };
  Op::JumpIfTrue(stack, frames);

  ASSERT_EQ(1, frames.size());
  ASSERT_EQ(5, frames[0].GetProgramCounter());
}

TEST(VMOperations, JumpIfTrueDoesNotModifyProgramCounterIfFalseIsInStack) {
  std::vector<VMValue> stack = { VMValue{ false } };

  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(5));
  VMFrame frame(&f);
  std::vector<VMFrame> frames = { frame };
  Op::JumpIfTrue(stack, frames);

  ASSERT_EQ(1, frames.size());
  ASSERT_EQ(1, frames[0].GetProgramCounter());
}

TEST(VMOperations, JumpIfTrueThrowsIfValueIsNonBoolean) {
  std::vector<VMValue> stack = { VMValue{ 5 } };

  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(5));
  VMFrame frame(&f);
  std::vector<VMFrame> frames = { frame };
  EXPECT_THROW(Op::JumpIfTrue(stack, frames), TypeError);
}


TEST(VMOperations, JumpIfFalseDoesNotModifyProgramCounterIfTrueIsInStack) {
  std::vector<VMValue> stack = { VMValue{ true } };

  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(5));
  VMFrame frame(&f);
  std::vector<VMFrame> frames = { frame };
  Op::JumpIfFalse(stack, frames);

  ASSERT_EQ(1, frames.size());
  ASSERT_EQ(1, frames[0].GetProgramCounter());
}

TEST(VMOperations, JumpIfFalseModifiesProgramCounterIfFalseIsInStack) {
  std::vector<VMValue> stack = { VMValue{ false } };

  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(5));
  VMFrame frame(&f);
  std::vector<VMFrame> frames = { frame };
  Op::JumpIfFalse(stack, frames);

  ASSERT_EQ(1, frames.size());
  ASSERT_EQ(5, frames[0].GetProgramCounter());
}

TEST(VMOperations, JumpIfFalseThrowsIfValueIsNonBoolean) {
  std::vector<VMValue> stack = { VMValue{ 'y' } };

  VMFunction f;
  f.AddByteCode(static_cast<ByteCode>(5));
  VMFrame frame(&f);
  std::vector<VMFrame> frames = { frame };
  EXPECT_THROW(Op::JumpIfFalse(stack, frames), TypeError);
}


// function invocations

TEST(VMOperations, InvokeMangedPushesFrameIntoStackWithCorrectFunctionAndInstructionCounterAtZero) {
  VMFunction dummy_function;

  VMFunction new_f;
  new_f.SetName("test_name");

  VMState state;
  state.AddFunction(dummy_function);
  state.AddFunction(new_f);


  VMFunction old_f;
  old_f.AddByteCode(static_cast<ByteCode>(1));

  std::vector<VMFrame> frames = { VMFrame{ &old_f } };
    
  Op::InvokeManaged(state, frames);
  ASSERT_EQ(2, frames.size());
  ASSERT_EQ(0, frames[1].GetProgramCounter());
  ASSERT_EQ(new_f.GetName(), frames[1].GetFunctionName());
}


TEST(VMOperations, InvokeManagedThrowsIfFrameStackOverflows) {
  VMFunction dummy_function;

  VMFunction new_f;
  new_f.SetName("test_name");

  VMState state;
  state.AddFunction(dummy_function);
  state.AddFunction(new_f);


  VMFunction old_f;
  old_f.AddByteCode(static_cast<ByteCode>(1));

  std::vector<VMFrame> frames;
  frames.resize(FRAME_SIZE);
  frames[FRAME_SIZE - 1] = VMFrame{ &old_f };
  EXPECT_THROW(Op::InvokeManaged(state, frames), FrameStackOverflowError);
}

TEST(VMOperations, InvokeMangedIndirectPushesFrameIntoStackWithCorrectFunctionAndInstructionCounterAtZero) {
  VMFunction dummy_function;

  VMFunction new_f;
  new_f.SetName("test_name");

  VMState state;
  state.AddFunction(dummy_function);
  state.AddFunction(new_f);


  VMFunction old_f;
  old_f.AddByteCode(static_cast<ByteCode>(0));

  std::vector<VMValue> stack = { VMValue{ 0 } };
  VMValue func;
  func.SetFunction(1);
  stack.push_back(func);

  std::vector<VMFrame> frames = { VMFrame{ &old_f } };

  Op::InvokeManagedIndirect(state, stack, frames);
  ASSERT_EQ(2, frames.size());
  ASSERT_EQ(0, frames[1].GetProgramCounter());
  ASSERT_EQ(new_f.GetName(), frames[1].GetFunctionName());
} 


TEST(VMOperations, InvokeManagedIndirectThrowsIfFrameStackOverflows) {
  VMFunction dummy_function;

  VMFunction new_f;
  new_f.SetName("test_name");

  VMState state;
  state.AddFunction(dummy_function);
  state.AddFunction(new_f);


  VMFunction old_f;
  old_f.AddByteCode(static_cast<ByteCode>(0));

  std::vector<VMValue> stack = { VMValue{ 0 } };
  VMValue func;
  func.SetFunction(1);
  stack.push_back(func);

  std::vector<VMFrame> frames;
  frames.resize(FRAME_SIZE);
  frames[FRAME_SIZE - 1] = VMFrame{ &old_f };
  EXPECT_THROW(Op::InvokeManagedIndirect(state, stack, frames), FrameStackOverflowError);
}

TEST(VMOperations, InvokeManagedIndirectThrowsIfFunctionIdIsInvalid) {
  VMFunction dummy_function;

  VMFunction new_f;
  new_f.SetName("test_name");

  VMState state;
  state.AddFunction(dummy_function);
  state.AddFunction(new_f);


  VMFunction old_f;
  old_f.AddByteCode(static_cast<ByteCode>(0));

  std::vector<VMValue> stack = { VMValue{ 0 } };
  VMValue func;
  func.SetFunction(21);
  stack.push_back(func);

  std::vector<VMFrame> frames = { VMFrame{ &old_f } };

  EXPECT_THROW(Op::InvokeManagedIndirect(state, stack, frames), InvalidFunctionIdError);
}

TEST(VMOperations, InvokeManagedIndirectThrowsIfFunctionArgumentCountsDoNotMatch) {
  VMFunction dummy_function;

  VMFunction new_f;
  new_f.SetName("test_name");
  new_f.SetArgumentCount(4);

  VMState state;
  state.AddFunction(dummy_function);
  state.AddFunction(new_f);


  VMFunction old_f;
  old_f.AddByteCode(static_cast<ByteCode>(0));

  std::vector<VMValue> stack = { VMValue{ 2 } };
  VMValue func;
  func.SetFunction(1);
  stack.push_back(func);

  std::vector<VMFrame> frames = { VMFrame{ &old_f } };

  EXPECT_THROW(Op::InvokeManagedIndirect(state, stack, frames), InvalidArgumentCountError);
}


int test_func(int a, int b) {
  return a + 2 * b;
}

TEST(VMOperations, InvokeNativeCallsTheNativeFunction) {

  VMState state;
  const std::string name = "testfunction";
  state.AddNativeBinding(name, CreateBinding(&test_func));

  std::vector<VMValue> stack = { VMValue{ 4 }, VMValue{ 5 }, ToManagedType(name) };
  
  VMFunction function;
  function.AddByteCode(static_cast<ByteCode>(2)); // arg count for native function
  std::vector<VMFrame> frames = { VMFrame {&function } };
  
  Op::InvokeNative(state, stack, frames);

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(ValueType::INT, stack[0].GetType());
  ASSERT_EQ(14, stack[0].AsInt());
}

TEST(VMOperations, InvokeNativeThrowsIfThereIsArgumentCountsDoNotMatch) {

  VMState state;
  const std::string name = "testfunction";
  state.AddNativeBinding(name, CreateBinding(&test_func));

  std::vector<VMValue> stack = { VMValue{ 4 }, VMValue{ 5 }, ToManagedType(name) };

  VMFunction function;
  function.AddByteCode(static_cast<ByteCode>(4)); // arg count for native function
  std::vector<VMFrame> frames = { VMFrame{ &function } };

  EXPECT_THROW(Op::InvokeNative(state, stack, frames), InvalidArgumentCountError);
}