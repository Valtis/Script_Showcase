#pragma once

namespace Compiler {

  class ArithmeticNode;

  class ASTVisitor {
  public:
    virtual ~ASTVisitor();
    virtual void Visit(ArithmeticNode *node) = 0;
  };

}
