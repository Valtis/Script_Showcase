#pragma once
#include "VM/Compiler/AST/AstNode.h"
#include "VM/Core/VMState.h"

#include <memory>
class ASTNode;
namespace Compiler {

  VMState GenerateCode(std::shared_ptr<ASTNode> root);

}