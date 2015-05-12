#include "VM/Compiler/Tokens/Token.h"

#include <memory>
namespace Compiler {

  /*
    Factory class that abstracts token creation.
  */
  namespace TokenFactory {
    std::unique_ptr<Token> CreateToken(TokenType type, int line, int column, std::string optionalValue = "");
  }
}
