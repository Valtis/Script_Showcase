#pragma once

#include "VMValue.h"
#include "VMFunction.h"
#include <vector>
#include <cstdint>
class VMClass {
public:
  uint32_t GetFieldCount();
  uint32_t GetFunctionCount();


  void SetFields(std::vector<ObjectType> fields);
  void SetFunctions(std::vector<VMFunction> functions);
  ObjectType GetFieldType(uint32_t field_index);
  VMFunction *GetFunction(uint32_t function_index);

  
private:
  std::vector<ObjectType> m_fields;
  std::vector<VMFunction> m_functions;
};