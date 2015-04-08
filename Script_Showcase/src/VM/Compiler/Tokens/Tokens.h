#pragma once
#include "VM/Compiler/Tokens/Token.h"

namespace Compiler {
#define TOKEN_WITH_NO_VALUE(CLASS_NAME, TO_STRING_TEXT) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column) : Token(type, line, column) { } \
    std::string ToString() override { return TO_STRING_TEXT; } \
  };

#define TOKEN_WITH_STRING(CLASS_NAME, TO_STRING_TEXT) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column, std::string value) : Token(type, line, column), m_string(value) { } \
    std::string ToString() override { return TO_STRING_TEXT + m_string; } \
    std::string GetString() { return m_string; } \
    private: \
      std::string m_string; \
  };

#define TOKEN_WITH_INTEGER(CLASS_NAME, TO_STRING_TEXT) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column, std::string value) : Token(type, line, column), m_integer(std::stoi(value)) { } \
    std::string ToString() override { return TO_STRING_TEXT + std::to_string(m_integer); } \
    int GetInteger() { return m_integer; } \
    private: \
      int m_integer; \
  };

#define TOKEN_WITH_DOUBLE(CLASS_NAME, TO_STRING_TEXT) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column, std::string value) : Token(type, line, column), m_double(std::stod(value)) { } \
    std::string ToString() override { return TO_STRING_TEXT + std::to_string(m_double); } \
    double GetDouble() { return m_double; } \
    private: \
      double m_double; \
  };

#define TOKEN_WITH_FLOAT(CLASS_NAME, TO_STRING_TEXT) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column, std::string value) : Token(type, line, column), m_float(std::stof(value)) { } \
    std::string ToString() override { return TO_STRING_TEXT + std::to_string(m_float); } \
    float GetFloat() { return m_float; } \
    private: \
      float m_float; \
  };



  TOKEN_WITH_NO_VALUE(LParenToken, "Left Parenthesis")
    TOKEN_WITH_NO_VALUE(RParenToken, "Right Parenthesis")
    TOKEN_WITH_NO_VALUE(PlusToken, "Plus")
    TOKEN_WITH_NO_VALUE(MinusToken, "Minus")
    TOKEN_WITH_NO_VALUE(MultiplyToken, "Multiply")
    TOKEN_WITH_NO_VALUE(DivideToken, "Divide")
    TOKEN_WITH_NO_VALUE(GreaterToken, "Greater")
    TOKEN_WITH_NO_VALUE(GreaterOrEqualToken, "Greater or equal")
    TOKEN_WITH_NO_VALUE(LessToken, "Less")
    TOKEN_WITH_NO_VALUE(LessOrEqualToken, "Less or equal")
    TOKEN_WITH_NO_VALUE(EqualToken, "Equal")
    TOKEN_WITH_NO_VALUE(IfToken, "If")
    TOKEN_WITH_NO_VALUE(SetValueToken, "SetValue")
    TOKEN_WITH_NO_VALUE(ReturnToken, "Return")
    TOKEN_WITH_NO_VALUE(InvokeNativeToken, "InvokeNative")
    TOKEN_WITH_NO_VALUE(FunctionToken, "FunctionToken")
    TOKEN_WITH_NO_VALUE(StaticsToken, "Statics")
    TOKEN_WITH_NO_VALUE(LocalsToken, "Locals")

    TOKEN_WITH_STRING(IdentifierToken, "Identifier: ");
    TOKEN_WITH_STRING(StringToken, "String: ");

    TOKEN_WITH_INTEGER(IntegerToken, "Integer: ");
    TOKEN_WITH_FLOAT(FloatToken, "Float: ");
    TOKEN_WITH_DOUBLE(DoubleToken, "Double: ");

}