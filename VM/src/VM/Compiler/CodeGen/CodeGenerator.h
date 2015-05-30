#pragma once
#include "VM/Compiler/AST/AstNode.h"
#include "VM/Core/VMState.h"

#include <memory>
class ASTNode;
/*
  Code generator. Takes the AST and outputs VMState
*/
namespace Compiler {

  VMState GenerateCode(std::shared_ptr<ASTNode> root);

}