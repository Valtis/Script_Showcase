#pragma once
#include <stdexcept>
#include <string>

#if _MSC_VER <= 1800
#define noexcept
#endif
#define EXCEPTION_CLASS(NAME____, SUPER_CLASS____) \
  class NAME____ : SUPER_CLASS____ {    \
    public: \
    NAME____(const std::string &what) : SUPER_CLASS____(what) { } \
      virtual const char* what() const noexcept { return (std::string(#SUPER_CLASS____) + ": " + SUPER_CLASS____::what()).c_str(); }\
    };

  EXCEPTION_CLASS(ValueStackOverFlowError, std::runtime_error)
  EXCEPTION_CLASS(ValueStackUnderFlowError, std::runtime_error)
  EXCEPTION_CLASS(FrameStackOverflowError, std::runtime_error)
  EXCEPTION_CLASS(FrameStackUnderFlowError, std::runtime_error)
  EXCEPTION_CLASS(ArrayIndexOutOfBoundsError, std::runtime_error)

  EXCEPTION_CLASS(TypeError, std::runtime_error)
  EXCEPTION_CLASS(InvalidConversionError, TypeError) 
  EXCEPTION_CLASS(InvalidArgumentCountError, std::runtime_error)

  EXCEPTION_CLASS(DivisionByZeroError, std::runtime_error)


  EXCEPTION_CLASS(InvalidFunctionIdError, std::logic_error) // this should not happen without either compiler bug or user manually messing with ids