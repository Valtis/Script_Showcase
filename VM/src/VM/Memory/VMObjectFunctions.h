#pragma once
#include "VM/Core/VMValue.h"
#include <cstdint>

/*
  Helper functions for arrays & objects. Used by garbage collector and memory manager
*/

#define ALIGN 4 // memory allocation aligned to multiples of 4. Allocations are padded as needed
const uint32_t HEAP_BEGIN_ADDRESS = ALIGN;
const uint32_t TYPE_POINTER_SIZE = 4;
const uint32_t FORWARD_POINTER_SIZE = 4;
const uint32_t ARRAY_LENGTH_FIELD_SIZE = 4;
const uint32_t VM_NULLPTR = 0;

namespace VMObjectFunction {
  // header size
  uint32_t ArrayHeaderSize();

  // aligns size into multiple of ALIGN bytes. For example, given align of 4 and size of 14, size is increased to 16 bytes.
  uint32_t AlignSize(uint32_t size);

  // gets type field from object. 
  uint32_t GetTypeField(VMValue object, uint8_t *memoryArea);


  // calculates the size of the pointed object. 
  uint32_t CalculateObjectSize(const VMValue object, uint8_t *memoryArea);
  
  // checks if typefield has the array flag bit set
  bool IsArray(uint32_t typeField);
  
  // gets the type of array (char array, int array, object array etc...)
  ValueType GetArrayValueType(uint32_t typeField);

  // gets the array length without performing any checks - this is an optimization used when we have already checked that the array is valid.
  uint32_t GetArrayLengthUnchecked(VMValue obj, uint8_t *memoryArea);
}