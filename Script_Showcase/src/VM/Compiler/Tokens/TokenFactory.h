#include "VM/Compiler/Tokens/Token.h"

#include <memory>
namespace Compiler {
  namespace TokenFactory {
    std::unique_ptr<Token> CreateToken(TokenType type, int line, int column, std::string optionalValue = "");
  }
}
