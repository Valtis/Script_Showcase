#pragma once
#include <stdexcept>
#include <string>

#if _MSC_VER <= 1800
#define noexcept
#endif
#define EXCEPTION_CLASS(NAME____, SUPER_CLASS____) \
  class NAME____ : public SUPER_CLASS____ {    \
    public: \
    NAME____(const std::string &what) : SUPER_CLASS____(std::string(#NAME____) + ": " + what) { } \
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


  EXCEPTION_CLASS(NoSuchNativeBindingError, std::runtime_error) 
  EXCEPTION_CLASS(InvalidFunctionIdError, std::logic_error) // this should not happen without either compiler bug or user manually messing with ids