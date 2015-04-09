#pragma once

namespace Compiler {

  class ArithmeticNode;
  class DoubleNode;
  class FloatNode;
  class FunctionNode;
  class IdentifierNode;
  class IntegerNode;
  class InvokeNativeNode;
  class RootNode;
  class SetValueNode;
  class StaticsNode;
  class StringNode;

  class ASTVisitor {
  public:
    virtual ~ASTVisitor();
    virtual void Visit(ArithmeticNode *node) = 0;
    virtual void Visit(DoubleNode *node) = 0;
    virtual void Visit(FloatNode *node) = 0;
    virtual void Visit(FunctionNode *node) = 0;
    virtual void Visit(IdentifierNode *node) = 0;
    virtual void Visit(InvokeNativeNode *node) = 0;
    virtual void Visit(IntegerNode *node) = 0;
    virtual void Visit(RootNode *node) = 0;
    virtual void Visit(SetValueNode *node) = 0;
    virtual void Visit(StaticsNode *node) = 0;
    virtual void Visit(StringNode *node) = 0;
  };

}
