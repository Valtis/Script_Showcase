#pragma once
#include "VM/Compiler/AST/ASTVisitor.h"
#include "VM/Core/VMState.h"
#include "VM/Core/VMFunction.h"

#include <memory>

namespace Compiler {
  class CodeGeneratorVisitor : public ASTVisitor {
  public:

    VMState AcquireState();
    
    void Visit(ArithmeticNode *node) override;
    void Visit(DoubleNode *node) override;
    void Visit(FloatNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(IdentifierNode *node) override;
    void Visit(IntegerNode *node) override;
    void Visit(InvokeNativeNode *node) override;
    void Visit(RootNode *node) override;
    void Visit(SetValueNode *node) override;
    void Visit(StaticsNode *node) override;
    void Visit(StringNode *node) override;

  private:
    VMState m_state;
    std::unordered_map <std::string, size_t> m_staticsNameMap;
    std::unordered_map <std::string, size_t> m_functionNameMap;
    std::unordered_map<std::string, size_t> m_localsNameMap;
    std::shared_ptr<VMFunction> m_current_function;
  };
}