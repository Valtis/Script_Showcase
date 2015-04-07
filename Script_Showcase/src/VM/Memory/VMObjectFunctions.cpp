#include "VM/Memory/VMObjectFunctions.h"

namespace VMObjectFunction {
  uint32_t ArrayMetaDataSize() {
    return TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE + ARRAY_LENGTH_FIELD_SIZE;
  }

  uint32_t AlignSize(uint32_t size) {
    return size + ((ALIGN - size % ALIGN) % ALIGN); // ensure alignment
  }

  uint32_t GetTypeField(VMValue object, uint8_t *memoryArea) {
    uint32_t *typeField = (uint32_t *)(memoryArea + object.as_managed_pointer());
    return *typeField;
  }


  bool IsArray(uint32_t typeField) {
    return (typeField & (1 << 31)) != 0;
  }

  ValueType GetArrayValueType(uint32_t typeField) {
    return static_cast<ValueType>(typeField & ~(1 << 31));
  }


  uint32_t CalculateObjectSize(const VMValue object, uint8_t *memoryArea) {
    auto typeField = GetTypeField(object, memoryArea);

    if (IsArray(typeField)) {
      return AlignSize(GetArrayLengthUnchecked(object, memoryArea)*TypeSize(GetArrayValueType(typeField)) + ArrayMetaDataSize());
    }
    else {
      throw std::logic_error("Size calculation not implemented for non-arrays");
    }
  }


  uint32_t GetArrayLengthUnchecked(VMValue object, uint8_t *memoryArea) {
    uint32_t *length_field = (uint32_t *)(memoryArea + object.as_managed_pointer()
      + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE);
    return *length_field;
  }

}