#include "VM/Compiler/Compiler.h"
#include "VM/Compiler/Tokenizer/Tokenizer.h"
#include "VM/Compiler/Parser/Parser.h"
#include "VM/Compiler/CodeGen/CodeGenerator.h"

namespace Compiler {
  VMState Compile(const std::string &path) {
    Tokenizer tokenizer;
    auto tokens = tokenizer.Tokenize(path);

    Parser parser(std::move(tokens));
    auto node = parser.Parse();

    return GenerateCode(node);

  }
}