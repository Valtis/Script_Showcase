#pragma once
#include "VM/Compiler/AST/ASTVisitor.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMFunction.h"

#include <memory>

namespace Compiler {

  class ASTNode;
  class CodeGeneratorVisitor : public ASTVisitor {
  public:

    VMState AcquireState();
    void AndOrHelper(ASTNode *node, ByteCode jumpInstructionAfterConditionEvaluation, bool returnValueIfAllPass);
    void Visit(AndNode *node) override;
    void Visit(ArithmeticNode *node) override;
    void Visit(ArrayNode *node) override;
    void Visit(ArrayLengthNode *node) override;
    void Visit(ComparisonNode *node) override;
    void Visit(CondNode *node) override;
    void Visit(DoubleNode *node) override;
    void Visit(ElseNode *node) override;
    void Visit(FloatNode *node) override;
    void Visit(FunctionCallNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(FunctionParameterListNode *node) override;
    void Visit(IdentifierNode *node) override;
    void Visit(IfNode *node) override;
    void Visit(IntegerNode *node) override;
    void Visit(InvokeNativeNode *node) override;
    void Visit(LocalsNode *node) override;
    void Visit(OrNode *node) override;
    void Visit(ReadArrayNode *node) override;
    void Visit(ReturnNode *node) override;
    void Visit(RootNode *node) override;
    void Visit(SetValueNode *node) override;
    void Visit(StaticsNode *node) override;
    void Visit(StringNode *node) override;
    void Visit(WhileNode *node) override;
    void Visit(WriteArrayNode * node) override;

  private:
    VMState m_state;
    
    void LocalVariableHelper(ASTNode *node); // adds variables to locals, used by function parameter list & function local declaration visitors
    
    std::unordered_map <std::string, size_t> m_staticsNameMap;
    std::unordered_map <std::string, size_t> m_functionNameMap;
    std::unordered_map <std::string, size_t> m_functionNameArgCountMap;

    std::unordered_map<std::string, size_t> m_localsNameMap;
    std::shared_ptr<VMFunction> m_current_function;
  };
}