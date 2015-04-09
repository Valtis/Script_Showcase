#include "VM/Compiler/Parser/Parser.h"
#include "VM/Compiler/Tokens/TokenFactory.h"
#include "VM/Compiler/Tokens/Tokens.h"
#include "VM/Compiler/AST/ArithmeticNode.h"
#include "VM/Compiler/AST/DoubleNode.h"
#include "VM/Compiler/AST/FloatNode.h"
#include "VM/Compiler/AST/FunctionNode.h"
#include "VM/Compiler/AST/IdentifierNode.h"
#include "VM/Compiler/AST/IntegerNode.h"
#include "VM/Compiler/AST/InvokeNativeNode.h"
#include "VM/Compiler/AST/RootNode.h"
#include "VM/Compiler/AST/SetValueNode.h"
#include "VM/Compiler/AST/StaticsNode.h"
#include "VM/Compiler/AST/StringNode.h"


#define AST_CAST(x) std::dynamic_pointer_cast<ASTNode>(x)

namespace Compiler {
  Parser::Parser(std::vector<std::unique_ptr<Token>> tokens) 
    : m_tokens(std::move(tokens)), m_position(0) {

  }

  std::shared_ptr<ASTNode> Parser::Parse() {
    auto rootNode = std::make_shared<RootNode>();

    ParseOptionalStaticsList(rootNode);
    while (true) {
      if (m_position == m_tokens.size()) {
        break;
      }
      ParseFunction(rootNode);
    }

    return rootNode;
  }

  void Parser::ParseOptionalStaticsList(std::shared_ptr<ASTNode> parent) {

    auto staticsNode = std::make_shared<StaticsNode>();
    parent->AddChildren(std::dynamic_pointer_cast<ASTNode>(staticsNode));

    auto token = Peek2();
    if (token && token->GetType() == TokenType::STATICS) {
      Expect(TokenType::LPAREN);
      Expect(TokenType::STATICS);
      ParseIdentifierList(staticsNode);
      Expect(TokenType::RPAREN);
    }
  }

  void Parser::ParseIdentifierList(std::shared_ptr<ASTNode> parent) {
    while (Peek() && Peek()->GetType() == TokenType::IDENTIFIER) {
      auto identifierNode = std::make_shared<IdentifierNode>();
      auto token = Advance();
      identifierNode->SetLine(token->GetLine());
      identifierNode->SetLine(token->GetColumn());
      identifierNode->SetName(dynamic_cast<StringToken *>(token)->GetValue());      
      parent->AddChildren(identifierNode);
    }
  }

  void Parser::ParseFunction(std::shared_ptr<ASTNode> parent) {
    auto functionNode = std::make_shared<FunctionNode>();
    parent->AddChildren(functionNode);
    
    Expect(TokenType::LPAREN);
    Expect(TokenType::FUNCTION);
    auto identifier = Expect(TokenType::IDENTIFIER);
    functionNode->SetName(dynamic_cast<IdentifierToken *>(identifier)->GetValue());
    Expect(TokenType::LPAREN);


    auto argumentListNode = std::make_shared<RootNode>();
    functionNode->AddChildren(argumentListNode);
    ParseIdentifierList(argumentListNode);
    Expect(TokenType::RPAREN);
    Expect(TokenType::LPAREN);
    ParseStatements(functionNode);
    Expect(TokenType::RPAREN);
  }

  void Parser::ParseStatements(std::shared_ptr<ASTNode> parent) {
    while (true) {
      if (Peek() && Peek()->GetType() != TokenType::LPAREN) {
        return;
      }

      ParseStatement(parent);

    }
  }
  
  void Parser::ParseStatement(std::shared_ptr<ASTNode> parent) {

    auto token = Peek2();
    if (token) {
      if (token->GetType() == TokenType::SET_VALUE) {
        ParseSetValue(parent);
      } else {
        ParseExpression(parent);
      }
    }

  }

  void Parser::ParseExpression(std::shared_ptr<ASTNode> parent) {
  
    auto token = Peek();
    if (token == nullptr) {
      throw std::runtime_error("Unexpected end-of-file while parsing an expression");
    }
    
    if (token->GetType() == TokenType::LPAREN) {
      auto innerToken = Peek2();
      if (innerToken == nullptr) {
        throw std::runtime_error("Unexpected end-of-file while parsing an expression");
      }
      switch (innerToken->GetType()) {
      case TokenType::PLUS:
      case TokenType::MINUS:
      case TokenType::MULTIPLY:
      case TokenType::DIVIDE:
        ParseArithmeticExpression(parent);
        break;
      case TokenType::INVOKE_NATIVE:
        ParseInvokeNative(parent);
      default:
        throw std::runtime_error("Unexpected token " + innerToken->ToString() + " at "
          + GetTokenPositionInfo(innerToken) + ". Expected arithmetic expression, function call or invokenatie");
      }
      return;
    } else {
      ParseLiteralOrIdentifier(parent);
    }

  }

  void Parser::ParseSetValue(std::shared_ptr<ASTNode> parent) {
    Expect(TokenType::LPAREN);
    auto token = Expect(TokenType::SET_VALUE);
    
    auto setValueNode = std::make_shared<SetValueNode>();
    setValueNode->SetLine(token->GetLine());
    setValueNode->SetColumn(token->GetColumn());
    ParseLiteralOrIdentifier(setValueNode);

    ParseExpression(parent);
    Expect(TokenType::RPAREN);
  }

  void Parser::ParseInvokeNative(std::shared_ptr<ASTNode> parent) {

    auto invokeNativeToken = Expect(TokenType::INVOKE_NATIVE); 
    
    auto invokeNativeNode = std::make_shared<InvokeNativeNode>();
    invokeNativeNode->SetLine(invokeNativeToken->GetLine());
    invokeNativeNode->SetColumn(invokeNativeToken->GetColumn());

    auto token = Peek();
    if (token == nullptr) {
      throw std::runtime_error("Unexpected end-of-file when parsing invokenative");
    }

    if (token->GetType() == TokenType::STRING || token->GetType() == TokenType::IDENTIFIER) {
      ParseLiteralOrIdentifier(invokeNativeNode);
    } else {
      throw std::runtime_error("Unexpected token " + token->ToString() + " Expected either string or identifer");
    }
    
    ParseArgumentList(invokeNativeNode);
  }

  void Parser::ParseArithmeticExpression(std::shared_ptr<ASTNode> parent) {
    Expect(TokenType::LPAREN);
    auto token = ExpectOneOf({ TokenType::PLUS, TokenType::MINUS, TokenType::MULTIPLY, TokenType::DIVIDE });
    auto exp = std::make_shared<ArithmeticNode>();
    exp->SetLine(token->GetLine());
    exp->SetColumn(token->GetColumn());
    exp->SetType(token->GetType());
    ParseArgumentList(exp);
    Expect(TokenType::RPAREN);
  }


  void Parser::ParseArgumentList(std::shared_ptr<ASTNode> parent) {
    while (true) {
      auto token = Peek();
      if (token == nullptr) {
        throw std::runtime_error("Unexpected end-of-file while parsing argument list");
      }
      if (token->GetType() == TokenType::RPAREN) {
        return;
      }
    
      ParseExpression(parent);
    }
  }

  void Parser::ParseLiteralOrIdentifier(std::shared_ptr<ASTNode> parent) {
    auto token = ExpectOneOf({TokenType::IDENTIFIER, TokenType::STRING, TokenType::INTEGER_NUMBER,
      TokenType::DOUBLE_NUMBER, TokenType::FLOAT_NUMBER });
    switch (token->GetType()) {
      case TokenType::IDENTIFIER:
      {
        auto identifier = std::make_shared<IdentifierNode>();
        identifier->SetLine(token->GetLine());
        identifier->SetColumn(token->GetColumn());
        identifier->SetName(dynamic_cast<IdentifierToken *>(token)->GetValue());
        parent->AddChildren(identifier);
      }
      break;

      case TokenType::STRING:
      {
        auto string = std::make_shared<StringNode>();
        string->SetLine(token->GetLine());
        string->SetColumn(token->GetColumn());
        string->SetValue(dynamic_cast<IdentifierToken *>(token)->GetValue());
        parent->AddChildren(string);
      }
      break;
      case TokenType::INTEGER_NUMBER:
      {
        auto number = std::make_shared<IntegerNode>();
        number->SetLine(token->GetLine());
        number->SetColumn(token->GetColumn());
        number->SetNumber(dynamic_cast<IntegerToken *>(token)->GetValue());
        parent->AddChildren(number);
      }
      break;

      case TokenType::DOUBLE_NUMBER:
      {
        auto number = std::make_shared<DoubleNode>();
        number->SetLine(token->GetLine());
        number->SetColumn(token->GetColumn());
        number->SetNumber(dynamic_cast<DoubleToken *>(token)->GetValue());
        parent->AddChildren(number);
      }
      break;
      case TokenType::FLOAT_NUMBER:
      {
        auto number = std::make_shared<FloatNode>();
        number->SetLine(token->GetLine());
        number->SetColumn(token->GetColumn());
        number->SetNumber(dynamic_cast<FloatToken *>(token)->GetValue());
        parent->AddChildren(number);
      }
      break;
      default:
        throw std::runtime_error("Unexpected token " + token->ToString() + " at " +GetTokenPositionInfo(token) +
          ". Expected literal or identifier");
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