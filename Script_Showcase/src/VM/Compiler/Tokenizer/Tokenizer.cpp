#include "VM/Compiler/Tokenizer/Tokenizer.h"
#include "VM/Compiler/Tokens/TokenFactory.h"
namespace Compiler {
 
  Tokenizer::Tokenizer() {
    m_singleCharSymbols['('] = TokenType::LPAREN;
    m_singleCharSymbols[')'] = TokenType::RPAREN;
    m_singleCharSymbols['+'] = TokenType::PLUS;
    m_singleCharSymbols['-'] = TokenType::MINUS;
    m_singleCharSymbols['*'] = TokenType::MULTIPLY;
    m_singleCharSymbols['/'] = TokenType::DIVIDE;
    m_singleCharSymbols['='] = TokenType::EQUAL;
    m_singleCharSymbols['>'] = TokenType::GREATER_THAN; 
    m_singleCharSymbols['<'] = TokenType::LESS_THAN;


    m_keyWords["mod"] = TokenType::IF;
    m_keyWords["while"] = TokenType::WHILE;
    m_keyWords["if"] = TokenType::IF;
    m_keyWords["cond"] = TokenType::COND;
    m_keyWords["and"] = TokenType::AND;
    m_keyWords["or"] = TokenType::OR;
    m_keyWords["invokenative"] = TokenType::INVOKE_NATIVE;
    m_keyWords["return"] = TokenType::RETURN;
    m_keyWords["setvalue"] = TokenType::SET_VALUE;
    m_keyWords["statics"] = TokenType::STATICS;
    m_keyWords["locals"] = TokenType::LOCALS;
    m_keyWords["fn"] = TokenType::FUNCTION;


  }

  std::vector<std::unique_ptr<Token>> Tokenizer::Tokenize(const std::string &path) {
    m_reader.Open(path);    
    m_tokens.clear();
    
    ParseTokens();

    return std::move(m_tokens);
  }

  void Tokenizer::ParseTokens() {
    while (true) {
      m_reader.SkipWhiteSpace();

     
      auto next_ch = m_reader.Peek();
      if (next_ch == EOF) {
        return;
      }
      
      if (StartsComment(next_ch)) {
        SkipComment();
      } else if (StartsSymbol(next_ch)) {
        ParseSymbol();
      } else if (StartsIdentifier(next_ch)) {
        ParseIdentifier();
      } else if (StartsString(next_ch)) {
        ParseString();
      } else if (StartsNumber(next_ch)) {
        ParseNumber();
      }
      else {
        throw std::runtime_error("Unrecognized token '" + std::string(1, (char)next_ch) + "' at line " 
          + std::to_string(m_reader.GetLine()) + ", column " + std::to_string(m_reader.GetColumn()));
      }
    }
  }

  bool Tokenizer::StartsComment(char ch) {
    return ch == '#';
  }


  // skip characters until eof or newline
  void Tokenizer::SkipComment() {
    while (true) {
      auto ch = m_reader.Get();
      if (ch == EOF || ch == '\n') {
        break;
      }
    }
  }

  bool Tokenizer::StartsSymbol(char ch) {
    return m_singleCharSymbols.find(ch) != m_singleCharSymbols.end();
  }

  void Tokenizer::ParseSymbol() {
    TokenType type;
    auto line = m_reader.GetLine();
    auto column = m_reader.GetColumn();

    auto ch = m_reader.Get();
    // check for multichar symbols
    if (ch == '<' && m_reader.Peek() == '=') {
      m_reader.Get();
      type = TokenType::LESS_OR_EQUAL_THAN;
    } else if (ch == '>' && m_reader.Peek() == '=') {
      m_reader.Get();
      type = TokenType::GREATER_OR_EQUAL_THAN;
    } else {
      type = m_singleCharSymbols[ch];
    }


    m_tokens.push_back(TokenFactory::CreateToken(type, line, column));
  }

  bool Tokenizer::StartsIdentifier(char ch) {
    return ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
  }

  void Tokenizer::ParseIdentifier() {

    auto line = m_reader.GetLine();
    auto column = m_reader.GetColumn();
    std::string identifier = "";
    while (true) {
      auto ch = m_reader.Peek();
      
      if (!(ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))) {
        break;
      }

      identifier += ch;
      m_reader.Get();
    }

    if (m_keyWords.find(identifier) != m_keyWords.end()) {
      m_tokens.push_back(TokenFactory::CreateToken(
        m_keyWords[identifier], line, column)); 
    } else {
      m_tokens.push_back(TokenFactory::CreateToken(
        TokenType::IDENTIFIER, m_reader.GetLine(), m_reader.GetColumn(), identifier));
    }
  }


  bool Tokenizer::StartsString(char ch) {
    return ch == '"';
  }

  void Tokenizer::ParseString() {
    auto line = m_reader.GetLine();
    auto column = m_reader.GetColumn();

    m_reader.Get(); // remove first '"'
    std::string str = "";
    while (true) {
      auto ch = m_reader.Get();
      if (ch == EOF) {
        throw std::runtime_error("Unexpected end-of-file when parsing string");
      } else if (ch == '"') {
        break;
      } else if (ch == '\\') {
        str += HandleEscapeSequence();
      } else {
        str += ch;
      }
    }

    m_tokens.push_back(TokenFactory::CreateToken(TokenType::STRING, line, column, str));
      
  }

  char Tokenizer::HandleEscapeSequence() {
    auto line = m_reader.GetLine();
    auto column = m_reader.GetColumn();
    auto ch = m_reader.Get();
    switch (ch) {
    case 'n':
      return '\n';
      break;
    case 't':
      return '\t';
      break;
    case '\\':
      return '\\';
    case '"':
      return '\"';
    case EOF:
      throw std::runtime_error("Unexpected end-of-file when parsing string");
    default:
      throw std::runtime_error("Invalid control character " + std::string(1, (char)ch) + " at line " +
        std::to_string(line) + " column " + std::to_string(column));
    }
  }

  bool Tokenizer::StartsNumber(char ch) {
    return (ch >= '0' && ch <= '9');
  }

  void Tokenizer::ParseNumber() {
    auto line = m_reader.GetLine();
    auto column = m_reader.GetColumn();
    auto type = TokenType::INTEGER_NUMBER;
    std::string number;
    bool dotFound = false;

    while (true) {
      auto ch = m_reader.Peek();
      if (ch >= '0' && ch <= '9') {
        number += ch;
      } else if (ch == '.') {
        if (dotFound) {
          break;
        }
        dotFound = true;
        number += ch;
        type = TokenType::DOUBLE_NUMBER;

      }  else if (ch == 'f') {
        type = TokenType::FLOAT_NUMBER;

      } else if (ch == 'd') {
        type = TokenType::DOUBLE_NUMBER;
        
      } else {
        break;
      }
      m_reader.Get();
    }
    m_tokens.push_back(TokenFactory::CreateToken(type, line, column, number));
  }

}
