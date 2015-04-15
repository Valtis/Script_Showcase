#include "VM/Compiler/CodeGen/CodeGenerator.h"
#include "VM/Compiler/CodeGen/CodeGeneratorVisitor.h"
#include "VM/Compiler/AST/ASTNode.h"



namespace Compiler {
  VMState GenerateCode(std::shared_ptr<ASTNode> root) {
    CodeGeneratorVisitor visitor;
    root->Accept(visitor);
    return visitor.AcquireState();
  }
}
