#include "VM/Compiler/CodeGen/CodeGeneratorVisitor.h"
#include "VM/Compiler/AST/AndNode.h"
#include "VM/Compiler/AST/ArithmeticNode.h"
#include "VM/Compiler/AST/ArrayNode.h"
#include "VM/Compiler/AST/ArrayLengthNode.h"
#include "VM/Compiler/AST/BooleanNode.h"
#include "VM/Compiler/AST/ComparisonNode.h"
#include "VM/Compiler/AST/CondNode.h"
#include "VM/Compiler/AST/DoubleNode.h"
#include "VM/Compiler/AST/ElseNode.h"
#include "VM/Compiler/AST/FloatNode.h"
#include "VM/Compiler/AST/FunctionCallNode.h"
#include "VM/Compiler/AST/FunctionNode.h"
#include "VM/Compiler/AST/FunctionParameterListNode.h"
#include "VM/Compiler/AST/IdentifierNode.h"
#include "VM/Compiler/AST/IfNode.h"
#include "VM/Compiler/AST/IntegerNode.h"
#include "VM/Compiler/AST/InvokeNativeNode.h"
#include "VM/Compiler/AST/LocalsNode.h"
#include "VM/Compiler/AST/NotNode.h"
#include "VM/Compiler/AST/OrNode.h"
#include "VM/Compiler/AST/ReadArrayNode.h"
#include "VM/Compiler/AST/ReturnNode.h"
#include "VM/Compiler/AST/RootNode.h"
#include "VM/Compiler/AST/SetValueNode.h"
#include "VM/Compiler/AST/StaticsNode.h"
#include "VM/Compiler/AST/StringNode.h"
#include "VM/Compiler/AST/WhileNode.h"
#include "VM/Compiler/AST/WriteArrayNode.h"

#include "VM/Core/ByteCode.h"
#include "VM/Core/VMValue.h"
#include "VM/Memory/MemoryManager.h"

namespace Compiler {

  VMState CodeGeneratorVisitor::AcquireState() {
    return std::move(m_state);
  }
  
  void CodeGeneratorVisitor::AndOrHelper(ASTNode *node, ByteCode jumpInstructionAfterConditionEvaluation, bool returnValueIfAllPass)
  {
    auto children = node->GetChildren();
    if (children.size() == 0) {
      throw std::runtime_error("Invalid argument count for or at " + node->GetPositionInfo());
    }
    std::vector<std::size_t> shortCircuitPlaceHolders;
    children[0]->Accept(*this);
    m_current_function->AddByteCode(jumpInstructionAfterConditionEvaluation);
    shortCircuitPlaceHolders.push_back(m_current_function->AddByteCode(ByteCode::NOP));

    for (size_t i = 1; i < children.size(); ++i) {
      children[i]->Accept(*this);
      m_current_function->AddByteCode(jumpInstructionAfterConditionEvaluation);
      shortCircuitPlaceHolders.push_back(m_current_function->AddByteCode(ByteCode::NOP));
    }

    // if these instructions are reached, all branches have been true(and) or false(or), and we must thus return true\false
    m_current_function->AddByteCode(ByteCode::PUSH_BOOLEAN);
    m_current_function->AddByteCode(static_cast<ByteCode>(returnValueIfAllPass));
    // jump over the true return branch
    m_current_function->AddByteCode(ByteCode::JUMP);
    auto endPlaceholder = m_current_function->AddByteCode(ByteCode::NOP);

    // update short circuit placeholders to next instruction
    for (auto index : shortCircuitPlaceHolders) {
      m_current_function->ChangeByteCode(index, static_cast<ByteCode>(m_current_function->GetByteCodeCount()));
    }

    m_current_function->AddByteCode(ByteCode::PUSH_BOOLEAN);
    m_current_function->AddByteCode(static_cast<ByteCode>(!returnValueIfAllPass));

    // and update the jump instruction which happens after pushing the boolean value when all passed into stack
    m_current_function->ChangeByteCode(endPlaceholder, static_cast<ByteCode>(m_current_function->GetByteCodeCount()));
  }

  void CodeGeneratorVisitor::Visit(AndNode *node) {
    AndOrHelper(node, ByteCode::JUMP_IF_FALSE, true);
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
    case TokenType::MOD:
      operation = ByteCode::MOD;
        break;
    default:
      throw std::logic_error("Internal compiler error: Invalid operation type with arithmetic node");
    }

    children[0]->Accept(*this);
    children[1]->Accept(*this);
    m_current_function->AddByteCode(operation);

    for (size_t i = 2; i < children.size(); ++i) {
      children[i]->Accept(*this);
      m_current_function->AddByteCode(operation);

    }
  }
  
  void CodeGeneratorVisitor::Visit(ArrayNode * node) {
    auto children = node->GetChildren();
    if (children.size() != 1) {
      throw std::runtime_error("Invalid argument count for array allocation at " + node->GetPositionInfo());
    }

    ByteCode code;
    switch (node->GetType()) {
    case TokenType::INTEGER_ARRAY:
      code = ByteCode::ALLOCATE_INTEGER_ARRAY;
      break;
    default:
      throw std::logic_error("Internal compiler error: Invalid type with array node");
    }

    children[0]->Accept(*this);
    m_current_function->AddByteCode(code);
  }

  void CodeGeneratorVisitor::Visit(ArrayLengthNode* node) {
    auto children = node->GetChildren();
    if (children.size() != 1) {
      throw std::runtime_error("Invalid argument count for arraylength at" + node->GetPositionInfo());
    }

    for (auto child : children) {
      child->Accept(*this);
    }

    m_current_function->AddByteCode(ByteCode::ARRAY_LENGTH);
  }

  void CodeGeneratorVisitor::Visit(BooleanNode *node) {
    m_current_function->AddByteCode(ByteCode::PUSH_BOOLEAN);
    m_current_function->AddByteCode(static_cast<ByteCode>(node->GetBoolean()));
  }

  void CodeGeneratorVisitor::Visit(ComparisonNode *node) {
    auto children = node->GetChildren();
    if (children.size() != 2) {
      throw std::runtime_error("Incorrect parameter count for comparison at " + node->GetPositionInfo());
    }
    ByteCode operation;
    switch (node->GetType()) {
    case TokenType::GREATER_THAN:
      operation = ByteCode::IS_GREATER;
      break;
    case TokenType::GREATER_OR_EQUAL_THAN:
      operation = ByteCode::IS_GREATER_OR_EQ;
      break;
    case TokenType::EQUAL:
      operation = ByteCode::IS_EQ;
      break;
    case TokenType::LESS_OR_EQUAL_THAN:
      operation = ByteCode::IS_LESS_OR_EQ;
      break;
    case TokenType::LESS_THAN:
      operation = ByteCode::IS_LESS;
      break;
    default:
      throw std::logic_error("Internal compiler error: Invalid operation type with comparison node");
    }

    children[0]->Accept(*this);
    children[1]->Accept(*this);
    m_current_function->AddByteCode(operation);
  }

  void CodeGeneratorVisitor::Visit(CondNode* node) {

    auto children = node->GetChildren();
    if (children.size() < 1) {
      throw std::runtime_error("Invalid parameter count for cond statement");
    }

    std::vector<size_t> falseJumpPlaceholders;
    std::vector<size_t> jumpPlaceholders;

    
    for (size_t i = 0; i < children.size(); ++i) {
       
      if (falseJumpPlaceholders.size() > 0) {
        m_current_function->ChangeByteCode(falseJumpPlaceholders[0], static_cast<ByteCode>(m_current_function->GetByteCodeCount()));
        falseJumpPlaceholders.pop_back();
      }

      if (i == children.size() - 1 && dynamic_cast<ElseNode *>(children.back()) != nullptr) {
        // if last node is infact else node, handle slightly differently; no need for condition evaluation or final jumps
        children.back()->GetChildren()[0]->Accept(*this);
        
        break;
      }

      auto condStatements = children[i]->GetChildren();
      // condition expression
      condStatements[0]->Accept(*this);
      m_current_function->AddByteCode(ByteCode::JUMP_IF_FALSE);
      falseJumpPlaceholders.push_back(m_current_function->AddByteCode(ByteCode::NOP));

      // statement block executed if condition was true
      condStatements[1]->Accept(*this);
      m_current_function->AddByteCode(ByteCode::JUMP);      
      jumpPlaceholders.push_back(m_current_function->AddByteCode(ByteCode::NOP));
    }

    if (falseJumpPlaceholders.size() > 0) {
      m_current_function->ChangeByteCode(falseJumpPlaceholders[0], static_cast<ByteCode>(m_current_function->GetByteCodeCount()));
      falseJumpPlaceholders.pop_back();
    }


    for (auto index : jumpPlaceholders) {
      m_current_function->ChangeByteCode(index, static_cast<ByteCode>(m_current_function->GetByteCodeCount()));
    }
  }

  void CodeGeneratorVisitor::Visit(DoubleNode *node) {

    m_current_function->AddByteCode(ByteCode::PUSH_DOUBLE);

    static_assert(sizeof(double) == sizeof(uint64_t), "Invalid assumption in casting");
    double number = node->GetNumber();
    auto val = *reinterpret_cast<uint64_t *>(&number);
    uint32_t mask_bit = 0;
    mask_bit--; // unsigned overflow is defined to wrap around, so we get uint32_max
    uint64_t mask = mask_bit;
    m_current_function->AddByteCode(static_cast<ByteCode>((val >> 32) & mask));
    m_current_function->AddByteCode(static_cast<ByteCode>(val & mask));

  }

  void CodeGeneratorVisitor::Visit(ElseNode* node) {
    throw std::logic_error("Else node should have been handled at higher level");
  }

  void CodeGeneratorVisitor::Visit(FloatNode *node) {
    m_current_function->AddByteCode(ByteCode::PUSH_FLOAT);
    static_assert(sizeof(float) == sizeof(uint32_t), "Invalid assumption in casting");
    auto number = node->GetNumber();
    auto val = *reinterpret_cast<uint32_t *>(&number);
    m_current_function->AddByteCode(static_cast<ByteCode>(val));

  }


  // TODO: Cleanup. 
  void CodeGeneratorVisitor::Visit(FunctionCallNode *node) {
    auto instruction = ByteCode::INVOKE_MANAGED;
    auto children = node->GetChildren();
    if (m_functionNameMap.find(node->GetName()) == m_functionNameMap.end()) {
      if (m_localsNameMap.find(node->GetName()) == m_localsNameMap.end() && m_staticsNameMap.find(node->GetName()) == m_staticsNameMap.end()) {
        // no function or variable with this name - error
        throw std::runtime_error("Usage of undeclared function " + node->GetName() + " at " + node->GetPositionInfo());
      } else {
        // there is a variable with this name - generate indirect call using this variable
        uint32_t id = 0;
        
        // push arguments
        for (auto child : children) {
          child->Accept(*this);
        }

        // push argument count so that this can be verified at runtime
        m_current_function->AddByteCode(ByteCode::PUSH_INTEGER);
        m_current_function->AddByteCode(static_cast<ByteCode>(children.size()));

        if (m_localsNameMap.find(node->GetName()) != m_localsNameMap.end()) {
          id = m_localsNameMap[node->GetName()];
          m_current_function->AddByteCode(ByteCode::LOAD_LOCAL);
          m_current_function->AddByteCode(static_cast<ByteCode>(id));

        }
        else if (m_staticsNameMap.find(node->GetName()) != m_staticsNameMap.end()) {
          id = m_staticsNameMap[node->GetName()];
          m_current_function->AddByteCode(ByteCode::LOAD_STATIC_OBJECT);
          m_current_function->AddByteCode(static_cast<ByteCode>(id));

        }
        m_current_function->AddByteCode(ByteCode::INVOKE_MANAGED_INDIRECT);
        return;
      }
    }

    // if this stage is reached, a function with this name exists, so generate direct call

    if (m_functionNameArgCountMap.find(node->GetName()) == m_functionNameArgCountMap.end()) {
      throw std::logic_error("Internal compiler error. No argument count recorded for function " + node->GetName());
    }

    if (m_functionNameArgCountMap[node->GetName()] != children.size()) {
      throw std::runtime_error("Invalid argument count for " + node->GetName() + ". Expected " +
        std::to_string(m_functionNameArgCountMap[node->GetName()]) + " parameters but " + std::to_string(children.size()) + " was given");
    }
    
    for (auto child : children) {
      child->Accept(*this);
    }

    m_current_function->AddByteCode(ByteCode::INVOKE_MANAGED);
   
    m_current_function->AddByteCode(static_cast<ByteCode>(m_functionNameMap[node->GetName()]));
      
  }

  void CodeGeneratorVisitor::Visit(FunctionNode *node) {
    auto children = node->GetChildren();
    for (size_t i = 0; i < children.size(); ++i) {
      children[i]->Accept(*this);
    }
  }


  void CodeGeneratorVisitor::LocalVariableHelper(ASTNode *node) {
    auto arguments = node->GetChildren();
    auto variableID = m_current_function->GetLocalCount(); // start ID number at current local count (variable ids start at zero)
    m_current_function->SetLocalCount(arguments.size() + m_current_function->GetLocalCount());

    for (size_t i = 0; i < arguments.size(); ++i, ++variableID) {
      auto name = dynamic_cast<IdentifierNode *>(arguments[i])->GetName();
      if (m_localsNameMap.find(name) != m_localsNameMap.end()) {
        throw std::runtime_error("Redeclaration of variable " + name + " at " + arguments[i]->GetPositionInfo());
      } 

      if (m_functionNameMap.find(name) != m_functionNameMap.end()) {
        throw std::runtime_error("Variable " + name + " shadows function with same name at " + arguments[i]->GetPositionInfo());
      }

      if (m_staticsNameMap.find(name) != m_staticsNameMap.end()) {
        throw std::runtime_error("Variable " + name + " shadows static variable with same name at " + arguments[i]->GetPositionInfo());
      }
      m_localsNameMap[name] = variableID;
    }
  }

  void CodeGeneratorVisitor::Visit(FunctionParameterListNode *node) {
    auto children = node->GetChildren();
    m_current_function->SetArgumentCount(children.size());
    m_functionNameArgCountMap[m_current_function->GetName()] = children.size();
    LocalVariableHelper(node);

    for (size_t i = 0; i < children.size(); ++i) {
      m_current_function->AddByteCode(ByteCode::STORE_LOCAL);
      m_current_function->AddByteCode(static_cast<ByteCode>(children.size() - 1 - i));
    }

  }



  void CodeGeneratorVisitor::Visit(IdentifierNode *node) {
    int id;

    auto name = node->GetName();
    if (m_localsNameMap.find(name) != m_localsNameMap.end()) {
      id = m_localsNameMap[name];
      m_current_function->AddByteCode(ByteCode::LOAD_LOCAL);
      m_current_function->AddByteCode(static_cast<ByteCode>(id));

    }
    else if (m_staticsNameMap.find(name) != m_staticsNameMap.end()) {
      id = m_staticsNameMap[name];
      m_current_function->AddByteCode(ByteCode::LOAD_STATIC_OBJECT);
      m_current_function->AddByteCode(static_cast<ByteCode>(id));

    }
    else if (m_functionNameMap.find(name) != m_functionNameMap.end()) {
      id = m_functionNameMap[name];
      m_current_function->AddByteCode(ByteCode::PUSH_FUNCTION);
      m_current_function->AddByteCode(static_cast<ByteCode>(id));
    }
    else {
      throw std::runtime_error("Usage of undeclared identifier " + name + " at " + node->GetPositionInfo());
    }

  }

  void CodeGeneratorVisitor::Visit(IfNode *node) {
    auto children = node->GetChildren();
    if (children.size() < 2 || children.size() > 3) {
      throw std::runtime_error("Incorrect parameter count for if statement " + node->GetPositionInfo());
    }

    // this is the boolean node
    children[0]->Accept(*this);
    // jump to false branch if boolean node was false
    m_current_function->AddByteCode(ByteCode::JUMP_IF_FALSE);
    auto false_branch_placeholder = m_current_function->AddByteCode(ByteCode::NOP);

    for (auto child : children[1]->GetChildren()) {
      child->Accept(*this);
    }

    if (children.size() == 3) {
      m_current_function->AddByteCode(ByteCode::JUMP);
      auto skip_false_branch_placeholder = m_current_function->AddByteCode(ByteCode::NOP);

      m_current_function->ChangeByteCode(false_branch_placeholder, static_cast<ByteCode>(m_current_function->GetByteCodeCount()));

      for (auto child : children[2]->GetChildren()) {
        child->Accept(*this);
      }

      m_current_function->ChangeByteCode(skip_false_branch_placeholder, static_cast<ByteCode>(m_current_function->GetByteCodeCount()));
    }
    else {
      m_current_function->ChangeByteCode(false_branch_placeholder, static_cast<ByteCode>(m_current_function->GetByteCodeCount()));
    }


  }

  void CodeGeneratorVisitor::Visit(IntegerNode *node) {

    m_current_function->AddByteCode(ByteCode::PUSH_INTEGER);
    m_current_function->AddByteCode(static_cast<ByteCode>(node->GetNumber()));
  }

  void CodeGeneratorVisitor::Visit(InvokeNativeNode *node) {
    auto children = node->GetChildren();
    if (children.size() == 0) {
      throw std::runtime_error("Invalid argument count for invokenative at " + node->GetPositionInfo());
    }


    for (size_t i = 1; i < children.size(); ++i) {
      children[i]->Accept(*this);
    }

    children[0]->Accept(*this);

    m_current_function->AddByteCode(ByteCode::INVOKE_NATIVE);
  }


  void CodeGeneratorVisitor::Visit(LocalsNode *node) {
    LocalVariableHelper(node);
  }

  void CodeGeneratorVisitor::Visit(NotNode* node) {
    auto children = node->GetChildren();
    if (children.size() != 1) {
      throw std::runtime_error("Invalid argument count for not at " + node->GetPositionInfo());
    }

    children[0]->Accept(*this);
    m_current_function->AddByteCode(ByteCode::NOT);
  }

  void CodeGeneratorVisitor::Visit(OrNode *node) {
    AndOrHelper(node, ByteCode::JUMP_IF_TRUE, false);

  }

  void CodeGeneratorVisitor::Visit(ReadArrayNode* node) {
    auto children = node->GetChildren();
    if (children.size() != 2) {
      throw std::runtime_error("Invalid argument count for arrayread at" + node->GetPositionInfo());
    }

    for (auto child : children) {
      child->Accept(*this);
    }

    m_current_function->AddByteCode(ByteCode::LOAD_ARRAY_INDEX);
  }

  void CodeGeneratorVisitor::Visit(ReturnNode* node) {
    auto children = node->GetChildren();
    for (auto child : children) {
      child->Accept(*this);
    }
    m_current_function->AddByteCode(ByteCode::RETURN);
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
      
      if (m_staticsNameMap.find(name) != m_staticsNameMap.end()) {
        throw std::runtime_error("Function " + name + " shadows static variable with same name at " + children[i]->GetPositionInfo());
      }
      m_functionNameMap[name] = id++;
    }

    // and then actually handle the functions
    for (size_t i = 1; i < children.size(); ++i) {
      m_current_function = std::make_shared<VMFunction>();
      m_current_function->SetName(dynamic_cast<FunctionNode *>(children[i])->GetName());
      m_localsNameMap.clear();

      children[i]->Accept(*this);

      // if last instruction of the function is not return, insert return instruction.
      if (children.size() > 0 && (children[i]->GetChildren().size() == 0 || dynamic_cast<ReturnNode *>(children[i]->GetChildren().back()) == nullptr)) {
        m_current_function->AddByteCode(ByteCode::RETURN);
      }

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

      m_current_function->AddByteCode(ByteCode::STORE_LOCAL);
      m_current_function->AddByteCode(static_cast<ByteCode>(id));
    }
    else if (m_staticsNameMap.find(name) != m_staticsNameMap.end()) {
      id = m_staticsNameMap[name];
      m_current_function->AddByteCode(ByteCode::STORE_STATIC_OBJECT);
      m_current_function->AddByteCode(static_cast<ByteCode>(id));
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
    // and the compiler is always run just before interpretation, the value is actually allocated here and pointer 
    // stored in VMstate. 

    auto ptr = MemMgrInstance().AllocateArray(ValueType::CHAR, node->GetValue().length());
    
    MemMgrInstance().WriteToArrayIndex(ptr, &node->GetValue()[0], 0, node->GetValue().length());
    
    int id = m_state.AddStaticObject(ptr); 

    m_current_function->AddByteCode(ByteCode::LOAD_STATIC_OBJECT);
    m_current_function->AddByteCode(static_cast<ByteCode>(id));
  }

  void CodeGeneratorVisitor::Visit(WhileNode *node) {
    auto children = node->GetChildren();
    if (children.size() < 2) {
      throw std::runtime_error("Incorrect parameter count for while statement " + node->GetPositionInfo());
    }

    auto startPosition = m_current_function->GetByteCodeCount();
    // this is the boolean node
    children[0]->Accept(*this);
    m_current_function->AddByteCode(ByteCode::JUMP_IF_FALSE);
    auto placeholderPosition = m_current_function->AddByteCode(ByteCode::NOP);


    for (size_t i = 1; i < children.size(); ++i) {
      children[i]->Accept(*this);
    }

    m_current_function->AddByteCode(ByteCode::JUMP);
    m_current_function->AddByteCode(static_cast<ByteCode>(startPosition));

    m_current_function->ChangeByteCode(placeholderPosition, static_cast<ByteCode>(m_current_function->GetByteCodeCount()));
  
  }

  void CodeGeneratorVisitor::Visit(WriteArrayNode* node) {
    auto children = node->GetChildren();
    if (children.size() != 3) {
      throw std::runtime_error("Invalid argument count for arraywrite at" + node->GetPositionInfo());
    }

    for (auto child : children) {
      child->Accept(*this);
    }

    m_current_function->AddByteCode(ByteCode::STORE_ARRAY_INDEX);
  }

}
