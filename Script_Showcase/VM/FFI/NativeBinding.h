#pragma once
#include "../VMValue.h"
#include "../VMOperations.h"
#include "ConversionFunctions.h"
#include "NativeBindingTypedef.h"
#include <functional>
#include <type_traits>
#include <array>


// thanks to MSN from stackoverflow. Slightly modified.
// http://stackoverflow.com/questions/5147492/member-function-call-in-decltype
template <typename R, typename C, typename... A>
R GetReturnType(R(C::*)(A...));


#define CREATE_0_ARGS_BINDING(CLASS, FUNCTION)  \
CreateNativeBinding<decltype(GetReturnType(&CLASS::FUNCTION)), \
                    CLASS>(std::mem_fn(&CLASS::FUNCTION))


#define CREATE_1_ARGS_BINDING(CLASS, FUNCTION, FIRST_PARAM)  \
CreateNativeBinding<decltype(GetReturnType(&CLASS::FUNCTION)), \
                    FIRST_PARAM,    \
                    CLASS>(std::mem_fn(&CLASS::FUNCTION))

#define CREATE_2_ARGS_BINDING(CLASS, FUNCTION, FIRST_PARAM, SECOND_PARAM)  \
CreateNativeBinding<decltype(GetReturnType(&CLASS::FUNCTION)), \
                    FIRST_PARAM,    \
                    SECOND_PARAM,   \
                    CLASS>(std::mem_fn(&CLASS::FUNCTION))


#define CREATE_3_ARGS_BINDING(CLASS, FUNCTION, FIRST_PARAM, SECOND_PARAM, THIRD_PARAM)  \
CreateNativeBinding<decltype(GetReturnType(&CLASS::FUNCTION)), \
                    FIRST_PARAM,    \
                    SECOND_PARAM,   \
                    THIRD_PARAM,    \
                    CLASS>(std::mem_fn(&CLASS::FUNCTION))

#define CREATE_4_ARGS_BINDING(CLASS, FUNCTION, FIRST_PARAM, SECOND_PARAM, THIRD_PARAM, FOURTH_PARAM)  \
CreateNativeBinding<decltype(GetReturnType(&CLASS::FUNCTION)), \
                    FIRST_PARAM,    \
                    SECOND_PARAM,   \
                    THIRD_PARAM,    \
                    FOURTH_PARAM,   \
                    CLASS>(std::mem_fn(&CLASS::FUNCTION))



// void functions

// no param
template <typename ReturnType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    pointer(classPointer);
  };
}


// 1 param
template <typename ReturnType,
  typename FirstParamType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto firstParam = ToNativeType<FirstParamType>(Op::PopValue(stack));

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    pointer(classPointer, firstParam);
  };
}

// 2 param
template <typename ReturnType,
  typename FirstParamType,
  typename SecondParamType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto secondParam = ToNativeType<SecondParamType>(Op::PopValue(stack));
    auto firstParam = ToNativeType<FirstParamType>(Op::PopValue(stack));

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    pointer(classPointer, firstParam, secondParam);
  };
}

// 3 param
template <typename ReturnType,
  typename FirstParamType,
  typename SecondParamType,
  typename ThirdParameterType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto thirdParam = ToNativeType<ThirdParameterType>(Op::PopValue(stack));
    auto secondParam = ToNativeType<SecondParamType>(Op::PopValue(stack));
    auto firstParam = ToNativeType<FirstParamType>(Op::PopValue(stack));

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    pointer(classPointer, firstParam, secondParam, thirdParam);
  };
}


// 4 param
template <typename ReturnType,
          typename FirstParamType, 
          typename SecondParamType, 
          typename ThirdParameterType,
          typename FourthParameterType,
          typename ClassType,
          typename MemberFunctionPointer,
          typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto fourthParam = ToNativeType<FourthParameterType>(Op::PopValue(stack));
    auto thirdParam = ToNativeType<ThirdParameterType>(Op::PopValue(stack));
    auto secondParam = ToNativeType<SecondParamType>(Op::PopValue(stack));
    auto firstParam = ToNativeType<FirstParamType>(Op::PopValue(stack));

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    pointer(classPointer, firstParam, secondParam, thirdParam, fourthParam);
  };
}





// non-void functions

// no param
template <typename ReturnType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename std::enable_if<!std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    Op::PushValue(pointer(classPointer), stack);
  };
}


// 1 param
template <typename ReturnType,
  typename FirstParamType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename std::enable_if<!std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto firstParam = ToNativeType<FirstParamType>(Op::PopValue(stack));

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    Op::PushValue(pointer(classPointer, firstParam), stack);
  };
}

// 2 param
template <typename ReturnType,
  typename FirstParamType,
  typename SecondParamType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename std::enable_if<!std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto secondParam = ToNativeType<SecondParamType>(Op::PopValue(stack));
    auto firstParam = ToNativeType<FirstParamType>(Op::PopValue(stack));

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    Op::PushValue(pointer(classPointer, firstParam, secondParam), stack);
  };
}

// 3 param
template <typename ReturnType,
  typename FirstParamType,
  typename SecondParamType,
  typename ThirdParameterType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename std::enable_if<!std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto thirdParam = ToNativeType<ThirdParameterType>(Op::PopValue(stack));
    auto secondParam = ToNativeType<SecondParamType>(Op::PopValue(stack));
    auto firstParam = ToNativeType<FirstParamType>(Op::PopValue(stack));

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    Op::PushValue(pointer(classPointer, firstParam, secondParam, thirdParam), stack);
  };
}


// 4 param
template <typename ReturnType,
  typename FirstParamType,
  typename SecondParamType,
  typename ThirdParameterType,
  typename FourthParameterType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename std::enable_if<!std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {

    auto fourthParam = ToNativeType<FourthParameterType>(Op::PopValue(stack));
    auto thirdParam = ToNativeType<ThirdParameterType>(Op::PopValue(stack));
    auto secondParam = ToNativeType<SecondParamType>(Op::PopValue(stack));
    auto firstParam = ToNativeType<FirstParamType>(Op::PopValue(stack));

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    Op::PushValue(pointer(classPointer, firstParam, secondParam, thirdParam, fourthParam), stack);
  };
}
