#include "VM/Compiler/Parser/Parser.h"
#include "VM/Compiler/Tokens/TokenFactory.h"
namespace Compiler {
  Parser::Parser(std::vector<std::unique_ptr<Token>> tokens) 
    : m_tokens(std::move(tokens)), m_position(0) {

  }

  void Parser::Parse() {

    ParseOptionalStaticsList();
    while (true) {
      if (m_position == m_tokens.size()) {
        break;
      }
      ParseFunction();
    }
  }

  void Parser::ParseOptionalStaticsList() {
    auto token = Peek2();
    if (token && token->GetType() == TokenType::STATICS) {
      Expect(TokenType::LPAREN);
      Expect(TokenType::STATICS);
      ParseIdentifierList();
      Expect(TokenType::RPAREN);
    }
  }

  void Parser::ParseIdentifierList() {
    while (Peek() && Peek()->GetType() == TokenType::IDENTIFIER) {
      Advance();
    }
  }

  void Parser::ParseFunction() {
    Expect(TokenType::LPAREN);
    Expect(TokenType::FUNCTION);
    Expect(TokenType::IDENTIFIER);
    Expect(TokenType::LPAREN);
    ParseIdentifierList();
    Expect(TokenType::RPAREN);
    Expect(TokenType::LPAREN);
    ParseStatements();
    Expect(TokenType::RPAREN);
  }

  void Parser::ParseStatements() {
    while (true) {
      if (Peek() && Peek()->GetType() != TokenType::LPAREN) {
        return;
      }

      Expect(TokenType::LPAREN);
      ParseStatement();

    }
  }
  
  void Parser::ParseStatement()
  {

    auto token = ExpectOneOf({ 
      TokenType::SET_VALUE, TokenType::INVOKE_NATIVE, TokenType::PLUS, TokenType::MINUS, 
      TokenType::MULTIPLY, TokenType::DIVIDE });


    switch (token->GetType()) {
    case TokenType::SET_VALUE:
      ParseSetValue();
      break;
    case TokenType::INVOKE_NATIVE:
      ParseInvokeNative();
      break;
    case TokenType::PLUS:
    case TokenType::MINUS:
    case TokenType::MULTIPLY:
    case TokenType::DIVIDE:
      ParseArithmeticExpression(token->GetType());
      break;
    default:
      throw std::runtime_error("Internal compiler error. Default case reached while parsing statement. Current token"
        + token->ToString());
    }


    Expect(TokenType::RPAREN);
  }


  void Parser::ParseSetValue() {
    Expect(TokenType::IDENTIFIER);
    auto token = ExpectOneOf({
      TokenType::IDENTIFIER,
      TokenType::DOUBLE_NUMBER,
      TokenType::FLOAT_NUMBER,
      TokenType::INTEGER_NUMBER,
      TokenType::STRING });
    
  }

  void Parser::ParseInvokeNative() {
    ExpectOneOf({ TokenType::STRING, TokenType::IDENTIFIER });
    ParseArgumentList();
  }

  void Parser::ParseArithmeticExpression(TokenType type) {
    ParseArgumentList();
  }


  void Parser::ParseArgumentList() {
    while (true) {
      if (Peek() && Peek()->GetType() == TokenType::RPAREN) {
        return;
      }
      auto token = ExpectOneOf({TokenType::IDENTIFIER, TokenType::DOUBLE_NUMBER, TokenType::FLOAT_NUMBER, 
        TokenType::INTEGER_NUMBER, TokenType::STRING, TokenType::LPAREN});
      
      switch (token->GetType()) {
        case TokenType::LPAREN:
          ParseStatement();
          break;
        default:
          ;
      }
    }
  }

  Token *Parser::Peek() {
    if (m_position >= m_tokens.size()) {
      return nullptr;
    }
    return m_tokens[m_position].get();
  }

  Token *Parser::Peek2() {
    if (m_position + 1 >= m_tokens.size()) {
      return nullptr;
    }
    return m_tokens[m_position+1].get();
  }

  Token *Parser::Advance() {
    if (m_position >= m_tokens.size()) {
      throw std::runtime_error("Unexpected end-of-file while parsing");
    }
    
    return m_tokens[m_position++].get();
  }

  Token *Parser::Expect(TokenType type) {
    Token *actual;
    try {
      actual = Advance();
    } catch (std::exception &ex) {
      throw std::runtime_error(std::string(ex.what()) + ". Expected token '" + TokenName(type) + "'");
    }
    if (actual->GetType() != type) {
      auto expected = TokenName(type);
      throw std::runtime_error("Unexpected token '" + actual->ToString() + "' at " 
        + GetTokenPositionInfo(actual) + ". Token '" + expected + "' was expected");
    }
    return actual;
  }

  Token *Parser::ExpectOneOf(std::vector<TokenType> tokenTypes) {
    auto actual = Advance();

    if (std::find(tokenTypes.begin(), tokenTypes.end(), actual->GetType()) == tokenTypes.end()) {
      std::string tokenList = "";
      for (auto type : tokenTypes) {
        tokenList += " '" + TokenName(type) + "'";
      }
      throw std::runtime_error("Unexpected token '" + actual->ToString() + "' at " +
        GetTokenPositionInfo(actual) + ". Expected one of" + tokenList);
    }
    return actual;
  }


  std::string Parser::GetTokenPositionInfo(const Token *token) {
    return std::string("line ") + std::to_string(token->GetLine())
      + " column " + std::to_string(token->GetColumn());
  }
}