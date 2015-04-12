#pragma once

namespace Compiler {

  class AndNode;
  class ArithmeticNode;
  class ComparisonNode;
  class CondNode;
  class DoubleNode;
  class ElseNode;
  class FloatNode;
  class FunctionNode;
  class FunctionParameterListNode;
  class IdentifierNode;
  class IfNode;
  class IntegerNode;
  class InvokeNativeNode;
  class LocalsNode;
  class OrNode;
  class RootNode;
  class SetValueNode;
  class StaticsNode;
  class StringNode;
  class WhileNode;

  class ASTVisitor {
  public:
    virtual ~ASTVisitor();

    virtual void Visit(AndNode *node) = 0;
    virtual void Visit(ArithmeticNode *node) = 0;
    virtual void Visit(ComparisonNode *node) = 0; 
    virtual void Visit(CondNode *node) = 0;
    virtual void Visit(DoubleNode *node) = 0;
    virtual void Visit(ElseNode *node) = 0;
    virtual void Visit(FloatNode *node) = 0;
    virtual void Visit(FunctionNode *node) = 0;
    virtual void Visit(FunctionParameterListNode *node) = 0;
    virtual void Visit(IdentifierNode *node) = 0;
    virtual void Visit(IfNode *node) = 0;
    virtual void Visit(InvokeNativeNode *node) = 0;
    virtual void Visit(IntegerNode *node) = 0;
    virtual void Visit(LocalsNode *node) = 0;
    virtual void Visit(OrNode *node) = 0;
    virtual void Visit(RootNode *node) = 0;
    virtual void Visit(SetValueNode *node) = 0;
    virtual void Visit(StaticsNode *node) = 0;
    virtual void Visit(StringNode *node) = 0;
    virtual void Visit(WhileNode *node) = 0;
  };

}
