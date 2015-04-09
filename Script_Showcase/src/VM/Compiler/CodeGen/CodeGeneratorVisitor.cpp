#include "VM/Compiler/CodeGen/CodeGeneratorVisitor.h"
#include "VM/Compiler/AST/ArithmeticNode.h"
#include "VM/Compiler/AST/DoubleNode.h"
#include "VM/Compiler/AST/FloatNode.h"
#include "VM/Compiler/AST/FunctionNode.h"
#include "VM/Compiler/AST/IdentifierNode.h"
#include "VM/Compiler/AST/IntegerNode.h"
#include "VM/Compiler/AST/RootNode.h"
#include "VM/Compiler/AST/StaticsNode.h"
#include "VM/Compiler/AST/StringNode.h"

#include "VM/Core/VMValue.h"
#include "VM/Memory/MemoryManager.h"

#include <iostream> // can be removed - debugging only
namespace Compiler {

  VMState CodeGeneratorVisitor::AcquireState() {
    return std::move(m_state);
  }

  void CodeGeneratorVisitor::Visit(ArithmeticNode *node)
  {
    auto children = node->GetChildren();    
    if (children.size() < 2) {
      throw std::runtime_error("Too few parameters for arithmetic node at " + node->GetPositionInfo());
    }
    std::string operation = "";
    switch (node->GetType()) {

    case TokenType::PLUS:
      operation = "ADD";
      break;
    case TokenType::MINUS:
      operation = "SUB";
      break; 
    case TokenType::MULTIPLY:
        operation = "MUL";
        break;
      case TokenType::DIVIDE:
        operation = "DIV";
        break;
    default:
      throw std::runtime_error("Internal compiler error: Invalid operation type with arithmetic node");
    }

    children[0]->Accept(*this);
    children[1]->Accept(*this);
    std::cout << operation << "\n";

    for (int i = 2; i < children.size(); ++i) {
      children[i]->Accept(*this);
      std::cout << operation << "\n";

    }
  }

  void CodeGeneratorVisitor::Visit(DoubleNode *node) {
    std::cout << "PUSH_DOUBLE " <<  node->GetNumber();
  }

  void CodeGeneratorVisitor::Visit(FloatNode *node) {
    std::cout << "PUSH_FLOAT " << node->GetNumber();
  }

  void CodeGeneratorVisitor::Visit(FunctionNode *node) {
    for (auto child : node->GetChildren()) {
      child->Accept(*this);
    }
  }

  void CodeGeneratorVisitor::Visit(IdentifierNode *node) {
    int id;

    auto name = node->GetName();
    if (m_localsNameMap.find(name) != m_localsNameMap.end()) {
      id = m_localsNameMap[node->GetName()];
      std::cout << "Identifier(local) " << node->GetName() << " id: " << id << "\n";
    } else if (m_staticsNameMap.find(name) != m_staticsNameMap.end()) {
      id = m_staticsNameMap[name];
      std::cout << "Identifier(static) " << node->GetName() << " id: " << id << "\n";
    } else {
      throw std::runtime_error("Usage of undeclared identifier " + name + " at " + node->GetPositionInfo());
    }

  }
  void CodeGeneratorVisitor::Visit(IntegerNode *node) {
    std::cout << "PUSH_INTEGER" << node->GetNumber();
  }

  void CodeGeneratorVisitor::Visit(RootNode *node) {
    
    auto children = node->GetChildren();
    // statics visitor    
    children[0]->Accept(*this);
    
    // peek function nodes. First get names so that ids can be decided
    int id = 0;

    for (int i = 1; i < children.size(); ++i) {
      auto name = dynamic_cast<FunctionNode *>(children[i])->GetName();
      if (m_functionNameMap.find(name) != m_functionNameMap.end()) {
        throw std::runtime_error("Redeclaration of function " + name + " at " + children[i]->GetPositionInfo());
      }
      m_functionNameMap[name] = id++;
    }
    // and then actually handle the functions
    for (int i = 1; i < children.size(); ++i) {
      m_current_function = std::make_shared<VMFunction>();
      m_current_function->SetName(dynamic_cast<FunctionNode *>(children[i])->GetName());
      
      children[i]->Accept(*this);

      // check that compiler generated id actually matches the id that VMState chose. Should only fail if
      // implementation is changed. (TODO: Let the compiler decide the id?)
      if (m_state.AddFunction(*m_current_function) != m_functionNameMap[m_current_function->GetName()]) {
        throw std::runtime_error("Internal compiler error. Function id mismatch");
      }
    }

  }

  void CodeGeneratorVisitor::Visit(StaticsNode *node) {
    m_staticsNameMap.clear();
    int id = 0;
    for (auto child : node->GetChildren()) {
      auto name = dynamic_cast<IdentifierNode *>(child)->GetName();
      if (m_staticsNameMap.find(name) != m_staticsNameMap.end()) {
        throw std::runtime_error("Redeclaration of static variable " + name + " at " + node->GetPositionInfo());
      }

      VMValue value;
      if (m_state.AddStaticObject(value) != id) {
        throw std::runtime_error("Internal compiler error. Static variable id mismatch");
      }


      m_staticsNameMap[name] = id++;
    }
  }

  void CodeGeneratorVisitor::Visit(StringNode *node) {
    // if the compiler wasn't this closely coupled with the VM, the string would be stored in a 
    // compiled data file read-only section and loaded at runtime. However, as this isn't the case
    // and the compiler is always run just before interpretation, the value is actually allocated and pointer 
    // stored in VMstate. 

    auto ptr = MemMgrInstance().AllocateArray(ValueType::CHAR, node->GetValue().length());
    MemMgrInstance().WriteToArrayIndex(ptr, &node->GetValue()[0], 0, node->GetValue().length());
    int id = m_state.AddStaticObject(ptr); 
    std::cout << "LoadStatic " << id << " (string)";

  }

}
