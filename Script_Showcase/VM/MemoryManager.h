#pragma once
#include "VMValue.h"



#define ALIGN 4 // memory allocation aligned to multiples of 4. Allocations are padded as needed

// visual studio 2013 and earlier do not support alignas
#if _MSC_VER <= 1800
#define alignas(x) __declspec(align(x))
#endif

class MemoryManager {
public:
  MemoryManager(uint32_t heap_size);
  MemoryManager(const MemoryManager &) = delete;
  MemoryManager(MemoryManager &&);
  MemoryManager &operator=(const MemoryManager &) = delete;
  MemoryManager &operator=(MemoryManager &&);
  ~MemoryManager();

  VMValue AllocateArray(const ObjectType objectType, const uint32_t length);
  void WriteToArrayIndex(const VMValue object, const void *value, const uint32_t index, const uint32_t length);
  void ReadFromArrayIndex(const VMValue object, void *value, const uint32_t index, const uint32_t length) const;

  void RunGc();

  uint32_t GetArrayLength(VMValue object) const;
private:

  void EnsureFreeMemory(uint32_t requiredSpace);

  struct ArrayReadWriteData {
    uint8_t *data;
    ObjectType type;
  };
  
  ArrayReadWriteData ArrayReadWriteCommon(const VMValue object, const uint32_t index, const uint32_t length) const;

  uint32_t GetArrayLengthUnchecked(VMValue obj) const;
  uint32_t GetTypeField(VMValue object) const;

  void EnsureNotNull(VMValue object) const;
  void EnsureArray(uint32_t type) const;
 
  alignas(ALIGN) uint8_t *m_memory;
  alignas(ALIGN) uint8_t *m_toSpace; // reseved for garbage collection

  uint32_t m_heapSize;
  uint32_t m_freeSpacePointer;
};


MemoryManager &MemMgrInstance();