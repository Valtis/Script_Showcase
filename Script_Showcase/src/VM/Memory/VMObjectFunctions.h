#pragma once
#include "VM/Core/VMValue.h"
#include <cstdint>

#define ALIGN 4 // memory allocation aligned to multiples of 4. Allocations are padded as needed
const uint32_t HEAP_BEGIN_ADDRESS = ALIGN;
const uint32_t TYPE_POINTER_SIZE = 4;
const uint32_t FORWARD_POINTER_SIZE = 4;
const uint32_t ARRAY_LENGTH_FIELD_SIZE = 4;
const uint32_t VM_NULLPTR = 0;

namespace VMObjectFunction {
  uint32_t ArrayMetaDataSize();

  uint32_t AlignSize(uint32_t size);

  uint32_t GetTypeField(VMValue object, uint8_t *memoryArea);

  uint32_t CalculateObjectSize(const VMValue object, uint8_t *memoryArea);
  
  bool IsArray(uint32_t typeField);
  
  ValueType GetArrayValueType(uint32_t typeField);

  uint32_t GetArrayLengthUnchecked(VMValue obj, uint8_t *memoryArea);
}