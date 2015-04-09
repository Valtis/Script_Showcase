#pragma once
#include "VM/Compiler/Tokens/Token.h"

namespace Compiler {
#define TOKEN_WITH_NO_VALUE(CLASS_NAME) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column) : Token(type, line, column) { } \
    std::string ToString() const override { return TokenName(GetType()); } \
  };

#define TOKEN_WITH_STRING(CLASS_NAME) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column, std::string value) : Token(type, line, column), m_string(value) { } \
    std::string ToString() const override { return TokenName(GetType()) + ": " + m_string; } \
    std::string GetValue() const { return m_string; } \
    private: \
      std::string m_string; \
  };

#define TOKEN_WITH_INTEGER(CLASS_NAME) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column, std::string value) : Token(type, line, column), m_integer(std::stoi(value)) { } \
    std::string ToString() const override { return TokenName(GetType()) + ": " + std::to_string(m_integer); } \
    int GetValue() const { return m_integer; } \
    private: \
      int m_integer; \
  };

#define TOKEN_WITH_DOUBLE(CLASS_NAME) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column, std::string value) : Token(type, line, column), m_double(std::stod(value)) { } \
    std::string ToString() const override { return TokenName(GetType()) + ": " + std::to_string(m_double); } \
    double GetValue() const { return m_double; } \
    private: \
      double m_double; \
  };

#define TOKEN_WITH_FLOAT(CLASS_NAME) \
  class CLASS_NAME : public Token { \
    public: \
    CLASS_NAME(TokenType type, int line, int column, std::string value) : Token(type, line, column), m_float(std::stof(value)) { } \
    std::string ToString() const override { return TokenName(GetType()) + ": " + std::to_string(m_float); } \
    float GetValue() const { return m_float; } \
    private: \
      float m_float; \
  };



    TOKEN_WITH_NO_VALUE(LParenToken)
    TOKEN_WITH_NO_VALUE(RParenToken)
    TOKEN_WITH_NO_VALUE(PlusToken)
    TOKEN_WITH_NO_VALUE(MinusToken)
    TOKEN_WITH_NO_VALUE(MultiplyToken)
    TOKEN_WITH_NO_VALUE(DivideToken)
    TOKEN_WITH_NO_VALUE(GreaterToken)
    TOKEN_WITH_NO_VALUE(GreaterOrEqualToken)
    TOKEN_WITH_NO_VALUE(LessToken)
    TOKEN_WITH_NO_VALUE(LessOrEqualToken)
    TOKEN_WITH_NO_VALUE(EqualToken)
    TOKEN_WITH_NO_VALUE(IfToken)
    TOKEN_WITH_NO_VALUE(SetValueToken)
    TOKEN_WITH_NO_VALUE(ReturnToken)
    TOKEN_WITH_NO_VALUE(InvokeNativeToken)
    TOKEN_WITH_NO_VALUE(FunctionToken)
    TOKEN_WITH_NO_VALUE(StaticsToken)
    TOKEN_WITH_NO_VALUE(LocalsToken)

    TOKEN_WITH_STRING(IdentifierToken);
    TOKEN_WITH_STRING(StringToken);

    TOKEN_WITH_INTEGER(IntegerToken);
    TOKEN_WITH_FLOAT(FloatToken);
    TOKEN_WITH_DOUBLE(DoubleToken);

}