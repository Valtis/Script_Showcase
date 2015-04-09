#pragma once
#include "VM/Compiler/AST/AstNode.h"
#include "VM/Core/VMState.h"

class ASTNode;
namespace Compiler {

  VMState GenerateCode(ASTNode *root);

}