#include "VM/Compiler/Parser/Parser.h"
#include "VM/Compiler/Tokens/Tokens.h"

#include "VM/Compiler/AST/AndNode.h"
#include "VM/Compiler/AST/ArithmeticNode.h"
#include "VM/Compiler/AST/ArrayNode.h"
#include "VM/Compiler/AST/ArrayLengthNode.h"
#include "VM/Compiler/AST/BooleanNode.h"
#include "VM/Compiler/AST/ComparisonNode.h"
#include "VM/Compiler/AST/CondNode.h"
#include "VM/Compiler/AST/DoubleNode.h"
#include "VM/Compiler/AST/ElseNode.h"
#include "VM/Compiler/AST/FloatNode.h"
#include "VM/Compiler/AST/FunctionCallNode.h"
#include "VM/Compiler/AST/FunctionNode.h"
#include "VM/Compiler/AST/FunctionParameterListNode.h"
#include "VM/Compiler/AST/IdentifierNode.h"
#include "VM/Compiler/AST/IfNode.h"
#include "VM/Compiler/AST/IntegerNode.h"
#include "VM/Compiler/AST/InvokeNativeNode.h"
#include "VM/Compiler/AST/LocalsNode.h"
#include "VM/Compiler/AST/OrNode.h"
#include "VM/Compiler/AST/ReadArrayNode.h"
#include "VM/Compiler/AST/ReturnNode.h"
#include "VM/Compiler/AST/RootNode.h"
#include "VM/Compiler/AST/SetValueNode.h"
#include "VM/Compiler/AST/StaticsNode.h"
#include "VM/Compiler/AST/StringNode.h"
#include "VM/Compiler/AST/WhileNode.h"
#include "VM/Compiler/AST/WriteArrayNode.h"


namespace Compiler {
  Parser::Parser(std::vector<std::unique_ptr<Token>> tokens) 
    : m_reader(std::move(tokens)) {
  }

  std::shared_ptr<ASTNode> Parser::Parse() {
    auto rootNode = std::make_shared<RootNode>();

    ParseOptionalStaticsList(rootNode);
    while (m_reader.HasTokens()) {
      ParseFunction(rootNode);
    }

    return rootNode;
  }

  void Parser::ParseOptionalStaticsList(std::shared_ptr<ASTNode> parent) { 
    
    auto staticsNode = std::make_shared<StaticsNode>();
    parent->AddChild(std::dynamic_pointer_cast<ASTNode>(staticsNode));

    auto token = m_reader.Peek2();
    if (token && token->GetType() == TokenType::STATICS) {
      m_reader.Expect(TokenType::LPAREN);
      m_reader.Expect(TokenType::STATICS);
      ParseIdentifierList(staticsNode);
      m_reader.Expect(TokenType::RPAREN);
    }
  }

  void Parser::ParseIdentifierList(std::shared_ptr<ASTNode> parent) {
    while (m_reader.Peek() && m_reader.Peek()->GetType() == TokenType::IDENTIFIER) {
      auto identifierNode = std::make_shared<IdentifierNode>();
      auto token = m_reader.Advance();
      identifierNode->SetLine(token->GetLine());
      identifierNode->SetColumn(token->GetColumn());
      identifierNode->SetName(dynamic_cast<IdentifierToken *>(token)->GetValue());      
      parent->AddChild(identifierNode);
    }
  }

  void Parser::ParseFunction(std::shared_ptr<ASTNode> parent) {
    auto functionNode = std::make_shared<FunctionNode>();

    parent->AddChild(functionNode);
    
    m_reader.Expect(TokenType::LPAREN);
    m_reader.Expect(TokenType::FUNCTION);
    auto identifier = m_reader.Expect(TokenType::IDENTIFIER);
    functionNode->SetName(dynamic_cast<IdentifierToken *>(identifier)->GetValue());
    functionNode->SetLine(identifier->GetLine());
    functionNode->SetColumn(identifier->GetColumn());

    m_reader.Expect(TokenType::LPAREN);


    auto argumentListNode = std::make_shared<FunctionParameterListNode>();
    functionNode->AddChild(argumentListNode);
    ParseIdentifierList(argumentListNode);
    m_reader.Expect(TokenType::RPAREN);
    ParseLocals(functionNode);
    m_reader.Expect(TokenType::LPAREN);
    ParseStatements(functionNode);
    m_reader.Expect(TokenType::RPAREN);
    m_reader.Expect(TokenType::RPAREN);
  }


  void Parser::ParseLocals(std::shared_ptr<ASTNode> parent) {
    auto node = m_reader.Peek2();
    if (node == nullptr || node->GetType() != TokenType::LOCALS) {
      return;
    }

    auto localsNode = std::make_shared<LocalsNode>();
    parent->AddChild(localsNode);
    m_reader.Expect(TokenType::LPAREN);
    m_reader.Expect(TokenType::LOCALS);
    ParseArgumentList(localsNode);
    m_reader.Expect(TokenType::RPAREN);

  }

  void Parser::ParseStatements(std::shared_ptr<ASTNode> parent) {
    while (true) {
      if (m_reader.Peek() && m_reader.Peek()->GetType() != TokenType::LPAREN) {
        return;
      }

      ParseStatement(parent);

    }
  }
  
  void Parser::ParseStatement(std::shared_ptr<ASTNode> parent) {
    auto token = m_reader.Peek2();
    if (token) {
      if (token->GetType() == TokenType::SET_VALUE) {
        ParseSetValue(parent);
      } else if (token->GetType() == TokenType::COND) {
        ParseCond(parent);
      } else if (token->GetType() == TokenType::IF) {
        ParseIf(parent);
      } else if (token->GetType() == TokenType::WHILE) {
        ParseWhile(parent);
      } else if (token->GetType() == TokenType::RETURN) {
        ParseReturn(parent);
      } else {
        ParseExpression(parent);
      }
    }
  }


  void Parser::ParseReturn(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.Expect(TokenType::RETURN);
    auto returnNode = std::make_shared<ReturnNode>();
    returnNode->SetLine(token->GetLine());
    returnNode->SetColumn(token->GetColumn());
    parent->AddChild(returnNode);

    auto next = m_reader.Peek();
    if (next == nullptr) {
      throw std::runtime_error("Unexpected end-of-file when parsing return statement");
    }

    if (next->GetType() == TokenType::RPAREN) {
      m_reader.Advance();
      return;
    }

    ParseStatement(returnNode);
    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseCond(std::shared_ptr<ASTNode> parent) {
    auto condNode = std::make_shared<CondNode>();
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.Expect(TokenType::COND);
    
    condNode->SetLine(token->GetLine());
    condNode->SetColumn(token->GetColumn());
    parent->AddChild(condNode);
    while (true) {
      auto next = m_reader.Peek();
      if (next) {
        if (next->GetType() == TokenType::RPAREN) {
          break;
        } 
        
        if (next->GetType() == TokenType::ELSE) {
          auto elseToken = m_reader.Advance();
          auto elseNode = std::make_shared<ElseNode>();
          elseNode->SetLine(elseToken->GetLine());
          elseNode->SetColumn(elseToken->GetColumn());
          

          condNode->AddChild(elseNode);
          
          auto statementsNode = std::make_shared<RootNode>();
          elseNode->AddChild(statementsNode);
          m_reader.Expect(TokenType::LPAREN);
          ParseStatements(statementsNode);
          m_reader.Expect(TokenType::RPAREN);
          break;
        }

        auto condStatementsNode = std::make_shared<RootNode>();
        condNode->AddChild(condStatementsNode);
        ParseExpression(condStatementsNode);


        m_reader.Expect(TokenType::LPAREN);
        auto statementsNode = std::make_shared<RootNode>();
        condStatementsNode->AddChild(statementsNode);
        ParseStatements(statementsNode);
        m_reader.Expect(TokenType::RPAREN);


      }

    }



    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseIf(std::shared_ptr<ASTNode> parent) {
    auto ifNode = std::make_shared<IfNode>();
    parent->AddChild(ifNode);
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.Expect(TokenType::IF);

    ifNode->SetLine(token->GetLine());
    ifNode->SetColumn(token->GetColumn());
    ParseExpression(ifNode);

    auto trueBranch = std::make_shared<RootNode>();
    ifNode->AddChild(trueBranch);

    auto falseBranch = std::make_shared<RootNode>();
    ifNode->AddChild(falseBranch);

    m_reader.Expect(TokenType::LPAREN);
    ParseStatements(trueBranch);
    m_reader.Expect(TokenType::RPAREN);

    if (m_reader.Peek() && m_reader.Peek()->GetType() == TokenType::LPAREN) {
      m_reader.Expect(TokenType::LPAREN);
      ParseStatements(falseBranch);
      m_reader.Expect(TokenType::RPAREN);
    }
    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseWhile(std::shared_ptr<ASTNode> parent) {
    auto whileNode = std::make_shared<WhileNode>();
    parent->AddChild(whileNode);
    
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.Expect(TokenType::WHILE);
    whileNode->SetLine(token->GetLine());
    whileNode->SetColumn(token->GetColumn());

    ParseExpression(whileNode);


    m_reader.Expect(TokenType::LPAREN);
    ParseStatements(whileNode);
    m_reader.Expect(TokenType::RPAREN);

    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseExpression(std::shared_ptr<ASTNode> parent) {
  
    auto token = m_reader.Peek();
    if (token == nullptr) {
      throw std::runtime_error("Unexpected end-of-file while parsing an expression");
    }
    
    if (token->GetType() == TokenType::LPAREN) {
      auto innerToken = m_reader.Peek2();
      if (innerToken == nullptr) {
        throw std::runtime_error("Unexpected end-of-file while parsing an expression");
      }

      switch (innerToken->GetType()) {
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULTIPLY:
        case TokenType::DIVIDE:
        case TokenType::MOD:
          ParseArithmeticExpression(parent);
          break;
        case TokenType::GREATER_OR_EQUAL_THAN:
        case TokenType::GREATER_THAN:
        case TokenType::EQUAL:
        case TokenType::LESS_OR_EQUAL_THAN:
        case TokenType::LESS_THAN:
          ParseComparisonExpression(parent);
          break;
        case TokenType::AND:
        case TokenType::OR:
          ParseAndOrExpression(parent);
          break;
        case TokenType::INVOKE_NATIVE:
          ParseInvokeNative(parent);
          break;
        case TokenType::IDENTIFIER:
          ParseFunctionCall(parent);
          break;
        case TokenType::INTEGER_ARRAY:
          ParseAllocateArray(parent);
          break;
        case TokenType::WRITE_ARRAY:
          ParseWriteArray(parent);
          break;
        case TokenType::READ_ARRAY:
          ParseReadArray(parent);
          break;
        case TokenType::ARRAY_LENGTH:
          ParseArrayLength(parent);
          break;
        default:
          throw std::runtime_error("Unexpected token " + innerToken->ToString() + " at "
            + innerToken->GetTokenPositionInfo() + ". Expected arithmetic expression, function call, comparison, array operation or invokenative");
      }
      return;
    } else {
      ParseLiteralOrIdentifier(parent);
    }

  }

  void Parser::ParseSetValue(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.Expect(TokenType::SET_VALUE);
    
    auto setValueNode = std::make_shared<SetValueNode>();
    parent->AddChild(setValueNode);
    setValueNode->SetLine(token->GetLine());
    setValueNode->SetColumn(token->GetColumn());
    
    auto idToken = m_reader.Expect(TokenType::IDENTIFIER);
    auto idNode = std::make_shared<IdentifierNode>();
    setValueNode->AddChild(idNode);
    idNode->SetLine(idToken->GetLine());
    idNode->SetColumn(idToken->GetColumn());
    idNode->SetName(dynamic_cast<IdentifierToken *>(idToken)->GetValue());

    ParseExpression(setValueNode);
    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseInvokeNative(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    auto invokeNativeToken = m_reader.Expect(TokenType::INVOKE_NATIVE); 
    
    auto invokeNativeNode = std::make_shared<InvokeNativeNode>();
    invokeNativeNode->SetLine(invokeNativeToken->GetLine());
    invokeNativeNode->SetColumn(invokeNativeToken->GetColumn());
    parent->AddChild(invokeNativeNode);
    auto token = m_reader.Peek();
    if (token == nullptr) {
      throw std::runtime_error("Unexpected end-of-file when parsing invokenative");
    }

    if (token->GetType() == TokenType::STRING || token->GetType() == TokenType::IDENTIFIER) {
      ParseLiteralOrIdentifier(invokeNativeNode);
    } else {
      throw std::runtime_error("Unexpected token " + token->ToString() + " Expected either string or identifer");
    }
    
    ParseArgumentList(invokeNativeNode);
    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseArithmeticExpression(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    
    auto token = m_reader.ExpectOneOf({ TokenType::PLUS, TokenType::MINUS, TokenType::MULTIPLY, TokenType::DIVIDE, TokenType::MOD });
    auto arithmeticNode = std::make_shared<ArithmeticNode>();
    
    parent->AddChild(arithmeticNode);
    arithmeticNode->SetLine(token->GetLine());
    arithmeticNode->SetColumn(token->GetColumn());
    arithmeticNode->SetType(token->GetType());

    ParseArgumentList(arithmeticNode);
    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseComparisonExpression(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN); 
    auto comparisonNode = std::make_shared<ComparisonNode>();
    parent->AddChild(comparisonNode);

    auto token = m_reader.ExpectOneOf({TokenType::GREATER_OR_EQUAL_THAN, TokenType::GREATER_THAN, 
      TokenType::EQUAL, TokenType::LESS_OR_EQUAL_THAN, TokenType::LESS_THAN});
      

    comparisonNode->SetLine(token->GetLine());
    comparisonNode->SetColumn(token->GetColumn());
    comparisonNode->SetType(token->GetType());

    ParseExpression(comparisonNode);
    ParseExpression(comparisonNode);
    
    m_reader.Expect(TokenType::RPAREN);
  }


  void Parser::ParseAndOrExpression(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    std::shared_ptr<ASTNode> andOrNode;

    auto token = m_reader.ExpectOneOf({ TokenType::AND, TokenType::OR });
    switch (token->GetType()) {
      case TokenType::AND:
        andOrNode = std::make_shared<AndNode>();
        break;
      case TokenType::OR:
        andOrNode = std::make_shared<OrNode>();
        break;
      default:
        throw std::logic_error("Default case reached in boolean expression parsing - this should not happen");
    }

    andOrNode->SetLine(token->GetLine());
    andOrNode->SetColumn(token->GetColumn());
    
    parent->AddChild(andOrNode);
    ParseExpression(andOrNode);
    ParseExpression(andOrNode);
    ParseArgumentList(andOrNode);
    

    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseFunctionCall(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.Expect(TokenType::IDENTIFIER);
    auto functionCallNode = std::make_shared<FunctionCallNode>();
    functionCallNode->SetName((dynamic_cast<IdentifierToken *>(token))->GetValue());
    functionCallNode->SetLine(token->GetLine());
    functionCallNode->SetColumn(token->GetColumn());
    parent->AddChild(functionCallNode);

    ParseArgumentList(functionCallNode);


    m_reader.Expect(TokenType::RPAREN);
  }


  void Parser::ParseArgumentList(std::shared_ptr<ASTNode> parent) {
    while (true) {
      auto token = m_reader.Peek();
      if (token == nullptr) {
        throw std::runtime_error("Unexpected end-of-file while parsing argument list");
      }
      if (token->GetType() == TokenType::RPAREN) {
        return;
      }
    
      ParseExpression(parent);
    }
  }

  void Parser::CreateIdentifierNode(std::shared_ptr<ASTNode> parent, Token* token) {
    auto identifier = std::make_shared<IdentifierNode>();
    identifier->SetLine(token->GetLine());
    identifier->SetColumn(token->GetColumn());
    identifier->SetName(dynamic_cast<IdentifierToken *>(token)->GetValue());
    parent->AddChild(identifier);
  }

  void Parser::ParseLiteralOrIdentifier(std::shared_ptr<ASTNode> parent) {
    int sign = 1;
    handle_minus:
    auto token = m_reader.ExpectOneOf({TokenType::IDENTIFIER, TokenType::STRING, TokenType::INTEGER_NUMBER,
      TokenType::DOUBLE_NUMBER, TokenType::FLOAT_NUMBER, TokenType::MINUS, TokenType::TRUE, TokenType::FALSE });

  
    switch (token->GetType()) {
      case TokenType::IDENTIFIER:
      {
        CreateIdentifierNode(parent, token);
      }
      break;

      case TokenType::STRING:
      {
        auto string = std::make_shared<StringNode>();
        string->SetLine(token->GetLine());
        string->SetColumn(token->GetColumn());
        string->SetValue(dynamic_cast<StringToken *>(token)->GetValue());
        parent->AddChild(string);
      }
      break;
      case TokenType::MINUS:
        sign = -1*sign;
        // yaay goto
        goto handle_minus;
      case TokenType::INTEGER_NUMBER:
      {
        auto number = std::make_shared<IntegerNode>();
        number->SetLine(token->GetLine());
        number->SetColumn(token->GetColumn());
        number->SetNumber(sign*dynamic_cast<IntegerToken *>(token)->GetValue());
        parent->AddChild(number);
      }
      break;

      case TokenType::DOUBLE_NUMBER:
      {
        auto number = std::make_shared<DoubleNode>();
        number->SetLine(token->GetLine());
        number->SetColumn(token->GetColumn());
        number->SetNumber(sign*dynamic_cast<DoubleToken *>(token)->GetValue());
        parent->AddChild(number);
      }
      break;
      case TokenType::FLOAT_NUMBER:
      {
        auto number = std::make_shared<FloatNode>();
        number->SetLine(token->GetLine());
        number->SetColumn(token->GetColumn());
        number->SetNumber(sign*dynamic_cast<FloatToken *>(token)->GetValue());
        parent->AddChild(number);
      }
      break;
      case TokenType::TRUE:
      case TokenType::FALSE:
      {
        auto booleanNode = std::make_shared<BooleanNode>();
        booleanNode->SetLine(token->GetLine());
        booleanNode->SetColumn(token->GetColumn());
        booleanNode->SetBoolean(token->GetType() == TokenType::TRUE);
        parent->AddChild(booleanNode);
      }
        break;
      default:
        throw std::runtime_error("Unexpected token " + token->ToString() + " at " + token->GetTokenPositionInfo() +
          ". Expected literal or identifier");
    }
  }


  void Parser::ParseAllocateArray(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.ExpectOneOf({TokenType::INTEGER_ARRAY});
    
    auto arrayNode = std::make_shared<ArrayNode>();
    arrayNode->SetLine(token->GetLine());
    arrayNode->SetColumn(token->GetColumn());
    arrayNode->SetType(token->GetType());

    parent->AddChild(arrayNode);
    ParseExpression(arrayNode);
    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseWriteArray(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.Expect(TokenType::WRITE_ARRAY);
    auto writeNode = std::make_shared<WriteArrayNode>();
    writeNode->SetLine(token->GetLine());
    writeNode->SetColumn(token->GetColumn());
    parent->AddChild(writeNode);
    auto idtoken = m_reader.Expect(TokenType::IDENTIFIER);
    CreateIdentifierNode(writeNode, idtoken);
    ParseExpression(writeNode);
    ParseExpression(writeNode);

    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseReadArray(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.Expect(TokenType::READ_ARRAY);
    auto readNode = std::make_shared<ReadArrayNode>();
    readNode->SetLine(token->GetLine());
    readNode->SetColumn(token->GetColumn());
    parent->AddChild(readNode);
    auto idtoken = m_reader.Expect(TokenType::IDENTIFIER);
    CreateIdentifierNode(readNode, idtoken);
    ParseExpression(readNode);
    m_reader.Expect(TokenType::RPAREN);
  }

  void Parser::ParseArrayLength(std::shared_ptr<ASTNode> parent) {
    m_reader.Expect(TokenType::LPAREN);
    auto token = m_reader.Expect(TokenType::ARRAY_LENGTH);
    auto arrayLengthNode = std::make_shared<ArrayLengthNode>();
    arrayLengthNode->SetLine(token->GetLine());
    arrayLengthNode->SetColumn(token->GetColumn());
    parent->AddChild(arrayLengthNode);
    auto idtoken = m_reader.Expect(TokenType::IDENTIFIER);
    CreateIdentifierNode(arrayLengthNode, idtoken);
    m_reader.Expect(TokenType::RPAREN);
  }



}