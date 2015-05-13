#include "VM/Compiler/Tokens/TokenFactory.h"
#include "VM/Compiler/Tokens/Tokens.h"

namespace Compiler {
  namespace TokenFactory {
    std::unique_ptr<Token> CreateToken(TokenType type, int line, int column, std::string optionalValue) {
      std::unique_ptr<Token> token;
      switch (type) {
        
      case TokenType::LPAREN:
        token.reset(new LParenToken(type, line, column));
        break;
      case TokenType::RPAREN:
        token.reset(new RParenToken(type, line, column));
        break;
      case TokenType::PLUS:
        token.reset(new PlusToken(type, line, column));
        break;
      case TokenType::MINUS:
        token.reset(new MinusToken(type, line, column));
        break;
      case TokenType::MULTIPLY:
        token.reset(new MultiplyToken(type, line, column));
        break;
      case TokenType::DIVIDE:
        token.reset(new DivideToken(type, line, column));
        break;
      case TokenType::MOD:
        token.reset(new ModToken(type, line, column));
        break;
      case TokenType::GREATER_THAN:
        token.reset(new GreaterToken(type, line, column));
        break;
      case TokenType::GREATER_OR_EQUAL_THAN:
        token.reset(new GreaterOrEqualToken(type, line, column));
        break;
      case TokenType::LESS_THAN:
        token.reset(new LessToken(type, line, column));
        break;
      case TokenType::LESS_OR_EQUAL_THAN:
        token.reset(new LessOrEqualToken(type, line, column));
        break;
      case TokenType::EQUAL:
        token.reset(new EqualToken(type, line, column));
        break;
      case TokenType::IDENTIFIER:
        token.reset(new IdentifierToken(type, line, column, optionalValue));
        break;
      case TokenType::STRING:
        token.reset(new StringToken(type, line, column, optionalValue));
        break;
      case TokenType::INTEGER_NUMBER:
        token.reset(new IntegerToken(type, line, column, optionalValue));
        break; 
      case TokenType::FLOAT_NUMBER:
        token.reset(new FloatToken(type, line, column, optionalValue));
        break; 
      case TokenType::DOUBLE_NUMBER:
        token.reset(new DoubleToken(type, line, column, optionalValue));
        break; 
      case TokenType::TRUE:
          token.reset(new TrueToken(type, line, column));
          break; 
      case TokenType::FALSE:
          token.reset(new FalseToken(type, line, column));
          break;
      case TokenType::IF:
        token.reset(new IfToken(type, line, column));
        break; 
      case TokenType::ELSE:
          token.reset(new ElseToken(type, line, column));
          break;
      case TokenType::COND:
        token.reset(new CondToken(type, line, column));
        break;
      case TokenType::OR:
        token.reset(new OrToken(type, line, column));
        break;
      case TokenType::AND:
        token.reset(new AndToken(type, line, column));
        break;
      case TokenType::WHILE:
        token.reset(new WhileToken(type, line, column));
        break;
      case TokenType::RETURN:
        token.reset(new ReturnToken(type, line, column));
        break;
      case TokenType::INVOKE_NATIVE:
        token.reset(new InvokeNativeToken(type, line, column));
        break;
      case TokenType::SET_VALUE:
        token.reset(new SetValueToken(type, line, column));
        break;
      case TokenType::FUNCTION:
        token.reset(new FunctionToken(type, line, column));
        break;
      case TokenType::LOCALS:
        token.reset(new LocalsToken(type, line, column));
        break;
      case TokenType::STATICS:
        token.reset(new StaticsToken(type, line, column));
        break; 
      case TokenType::INTEGER_ARRAY:
        token.reset(new IntegerArrayToken(type, line, column));
        break;
      case TokenType::WRITE_ARRAY:
        token.reset(new WriteArrayToken(type, line, column));
        break;
      case TokenType::READ_ARRAY:
        token.reset(new ReadArrayToken(type, line, column));
        break;
      case TokenType::ARRAY_LENGTH:
        token.reset(new ArrayLengthToken(type, line, column));
        break;
      default: 
        throw std::runtime_error("Unrecognized token type id " + std::to_string(static_cast<int>(type)));
      }

      return token;
    }

  }
}

