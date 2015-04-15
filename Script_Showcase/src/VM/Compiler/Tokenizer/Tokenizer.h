#pragma once

#include "VM/Compiler/Tokenizer/Reader.h"
#include "VM/Compiler/Tokens/Token.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
namespace Compiler {
  class Tokenizer {
  public:
    Tokenizer();
    std::vector<std::unique_ptr<Token>> Tokenize(const std::string &path);
  private:
    void ParseTokens();
    bool StartsComment(char ch);
    void SkipComment();

    bool StartsSymbol(char ch);
    void ParseSymbol();

    bool StartsIdentifier(char ch);
    void ParseIdentifier();

    bool StartsString(char ch);
    void ParseString();
    char HandleEscapeSequence();

    bool StartsNumber(char ch);
    void ParseNumber();

    std::vector<std::unique_ptr<Token>> m_tokens;

    std::unordered_map<char, TokenType> m_singleCharSymbols;
    std::unordered_map<std::string, TokenType> m_keyWords;
    Reader m_reader;
  };
}