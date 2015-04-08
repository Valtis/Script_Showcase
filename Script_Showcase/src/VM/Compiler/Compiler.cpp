#include "VM/Compiler/Compiler.h"
#include "VM/Compiler/Tokenizer/Tokenizer.h"
#include "VM/Compiler/Tokens/Token.h"
#include "VM/Compiler/Parser/Parser.h"

#include <iostream>
namespace Compiler {
  VMState Compile(const std::string &path) {
    Tokenizer tokenizer;
    auto tokens = tokenizer.Tokenize(path);

    std::cout << "Tokens:\n";
    for (auto &token : tokens) {
      std::cout << token->ToString() << "\n";
    }
    Parser parser(std::move(tokens));
    parser.Parse();

    return VMState{ "path" };
  }
}