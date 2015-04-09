#include "VM/Compiler/CodeGen/CodeGeneratorVisitor.h"
#include "VM/Compiler/AST/ArithmeticNode.h"
#include "VM/Compiler/AST/DoubleNode.h"
#include "VM/Compiler/AST/FloatNode.h"
#include "VM/Compiler/AST/FunctionNode.h"
#include "VM/Compiler/AST/IdentifierNode.h"
#include "VM/Compiler/AST/IntegerNode.h"
#include "VM/Compiler/AST/InvokeNativeNode.h"
#include "VM/Compiler/AST/RootNode.h"
#include "VM/Compiler/AST/SetValueNode.h"
#include "VM/Compiler/AST/StaticsNode.h"
#include "VM/Compiler/AST/StringNode.h"

#include "VM/Core/ByteCode.h"
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
    ByteCode operation;
    switch (node->GetType()) {

    case TokenType::PLUS:
      operation = ByteCode::ADD;
      break;
    case TokenType::MINUS:
      operation = ByteCode::SUB;
      break; 
    case TokenType::MULTIPLY:
      operation = ByteCode::MUL;
        break;
      case TokenType::DIVIDE:
        operation = ByteCode::DIV;
        break;
    default:
      throw std::runtime_error("Internal compiler error: Invalid operation type with arithmetic node");
    }

    children[0]->Accept(*this);
    children[1]->Accept(*this);
    std::cout << operation << "\n";

    for (size_t i = 2; i < children.size(); ++i) {
      children[i]->Accept(*this);
      std::cout << operation << "\n";

    }
  }

  void CodeGeneratorVisitor::Visit(DoubleNode *node) {
    std::cout << "PUSH_DOUBLE " << node->GetNumber() << "\n";
  }

  void CodeGeneratorVisitor::Visit(FloatNode *node) {
    std::cout << "PUSH_FLOAT " << node->GetNumber() << "\n";
  }

  void CodeGeneratorVisitor::Visit(FunctionNode *node) {
    auto children = node->GetChildren();
    auto argumentList = children[0]->GetChildren();
    m_current_function->SetLocalCount(argumentList.size());

    for (size_t i = 0; i < argumentList.size(); ++i) {
      auto name = dynamic_cast<IdentifierNode *>(argumentList[i])->GetName();
      if (m_localsNameMap.find(name) != m_localsNameMap.end()) {
        throw std::runtime_error("Redeclaration of function argument " + name + " at " + argumentList[i]->GetPositionInfo());
      }
      m_localsNameMap[name] = i;

    }
    for (size_t i = 1; i < children.size(); ++i) {
      children[i]->Accept(*this);
    }
  }

  void CodeGeneratorVisitor::Visit(IdentifierNode *node) {
    int id;

    auto name = node->GetName();
    if (m_localsNameMap.find(name) != m_localsNameMap.end()) {
      id = m_localsNameMap[name];
      std::cout << "Identifier(local) " << name << " id: " << id << "\n";
    } else if (m_staticsNameMap.find(name) != m_staticsNameMap.end()) {
      id = m_staticsNameMap[name];
      std::cout << "Identifier(static) " << name << " id: " << id << "\n";
    } else {
      throw std::runtime_error("Usage of undeclared identifier " + name + " at " + node->GetPositionInfo());
    }

  }
  void CodeGeneratorVisitor::Visit(IntegerNode *node) {
    std::cout << "PUSH_INTEGER " << node->GetNumber() << "\n";
  }
  
  void CodeGeneratorVisitor::Visit(InvokeNativeNode *node) {
    auto children =  node->GetChildren();
    if (children.size() == 0) {
      throw std::runtime_error("Invalid argument count for invokenative at " + node->GetPositionInfo());
    }


    for (size_t i = 1; i < children.size(); ++i) {
      children[i]->Accept(*this);
    }

    children[0]->Accept(*this);

    std::cout << "InvokeNative\n";
  }

  void CodeGeneratorVisitor::Visit(RootNode *node) {
    
    auto children = node->GetChildren();
    // statics visitor    
    children[0]->Accept(*this);
    
    // peek function nodes. First get names so that ids can be decided
    int id = 0;

    for (size_t i = 1; i < children.size(); ++i) {
      auto name = dynamic_cast<FunctionNode *>(children[i])->GetName();
      if (m_functionNameMap.find(name) != m_functionNameMap.end()) {
        throw std::runtime_error("Redeclaration of function " + name + " at " + children[i]->GetPositionInfo());
      }
      m_functionNameMap[name] = id++;
    }
    // and then actually handle the functions
    for (size_t i = 1; i < children.size(); ++i) {
      m_current_function = std::make_shared<VMFunction>();
      m_current_function->SetName(dynamic_cast<FunctionNode *>(children[i])->GetName());
      m_localsNameMap.clear();

      children[i]->Accept(*this);

      // check that compiler generated id actually matches the id that VMState chose. Should only fail if
      // implementation is changed. (TODO: Let the compiler decide the id?)
      if (m_state.AddFunction(*m_current_function) != m_functionNameMap[m_current_function->GetName()]) {
        throw std::runtime_error("Internal compiler error. Function id mismatch");
      }
    }

  }
  
  void CodeGeneratorVisitor::Visit(SetValueNode *node) {
    auto children = node->GetChildren();
    if (children.size() != 2) {
      throw std::runtime_error("Invalid argument count for setvalue at " + node->GetPositionInfo());
    }
    children[1]->Accept(*this);
    
    int id;
    auto identifierNode = dynamic_cast<IdentifierNode *>(children[0]);
    auto name = identifierNode->GetName();
    if (m_localsNameMap.find(name) != m_localsNameMap.end()) {
      id = m_localsNameMap[name];
      std::cout << "Store to Identifier(local) " << name<< " id: " << id << "\n";
    }
    else if (m_staticsNameMap.find(name) != m_staticsNameMap.end()) {
      id = m_staticsNameMap[name];
      std::cout << "Store to Identifier(static) " << name << " id: " << id << "\n";
    }
    else {
      throw std::runtime_error("Usage of undeclared identifier " + name + " at " + node->GetPositionInfo());
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
