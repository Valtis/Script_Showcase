#pragma once

namespace Compiler {

  class AndNode;
  class ArithmeticNode;
  class ArrayNode;
  class ArrayLengthNode;
  class BooleanNode;
  class ComparisonNode;
  class CondNode;
  class DoubleNode;
  class ElseNode;
  class FloatNode;
  class FunctionCallNode;
  class FunctionNode;
  class FunctionParameterListNode;
  class IdentifierNode;
  class IfNode;
  class IntegerNode;
  class InvokeNativeNode;
  class LocalsNode;
  class NotNode;
  class OrNode;
  class ReadArrayNode;
  class ReturnNode;
  class RootNode;
  class SetValueNode;
  class StaticsNode;
  class StringNode;
  class WhileNode;
  class WriteArrayNode;

  class ASTVisitor {
  public:
    virtual ~ASTVisitor();

    virtual void Visit(AndNode *node) = 0;
    virtual void Visit(ArithmeticNode *node) = 0;
    virtual void Visit(ArrayNode *node) = 0;
    virtual void Visit(ArrayLengthNode *node) = 0;
    virtual void Visit(BooleanNode *node) = 0;
    virtual void Visit(ComparisonNode *node) = 0; 
    virtual void Visit(CondNode *node) = 0;
    virtual void Visit(DoubleNode *node) = 0;
    virtual void Visit(ElseNode *node) = 0;
    virtual void Visit(FloatNode *node) = 0;
    virtual void Visit(FunctionCallNode *node) = 0;
    virtual void Visit(FunctionNode *node) = 0;
    virtual void Visit(FunctionParameterListNode *node) = 0;
    virtual void Visit(IdentifierNode *node) = 0;
    virtual void Visit(IfNode *node) = 0;
    virtual void Visit(InvokeNativeNode *node) = 0;
    virtual void Visit(IntegerNode *node) = 0;
    virtual void Visit(LocalsNode *node) = 0;
    virtual void Visit(NotNode *node) = 0;
    virtual void Visit(OrNode *node) = 0;
    virtual void Visit(ReadArrayNode *node) = 0;
    virtual void Visit(ReturnNode *node) = 0;
    virtual void Visit(RootNode *node) = 0;
    virtual void Visit(SetValueNode *node) = 0;
    virtual void Visit(StaticsNode *node) = 0;
    virtual void Visit(StringNode *node) = 0;
    virtual void Visit(WhileNode *node) = 0;
    virtual void Visit(WriteArrayNode *node) = 0;
  };

}
