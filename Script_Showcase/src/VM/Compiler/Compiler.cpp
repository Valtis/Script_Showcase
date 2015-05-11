#include "VM/Compiler/Compiler.h"
#include "VM/Compiler/Tokenizer/Tokenizer.h"
#include "VM/Compiler/Parser/Parser.h"
#include "VM/Compiler/CodeGen/CodeGenerator.h"
#include "VM/Compiler/Optimizer/ByteCodeOptimizer.h"
#include "VM/Compiler/ByteCodePrinter/ByteCodePrinter.h"


namespace Compiler {
  VMState Compile(const std::string &path) {
    Tokenizer tokenizer;
    auto tokens = tokenizer.Tokenize(path);

    Parser parser{ std::move(tokens) };
    auto node = parser.Parse();

    auto state = GenerateCode(node);

    ByteCodeOptimizer::OptimizeByteCode(state);

    ByteCodePrinter printer;
    printer.PrintByteCode(state, path);
    return state;
  }
}