#pragma once

#include "VM/Core/VMValue.h"
#include "VM/Memory/RootSetProvider.h"
#include "VM/Memory/GarbageCollector.h"
#include "VM/Memory/VMObjectFunctions.h"
#include <vector>
#include <memory>





// visual studio 2013 and earlier do not support alignas
#if _MSC_VER <= 1800
#define alignas(x) __declspec(align(x))
#endif

class MemoryManager {
public:
  MemoryManager(uint32_t heap_size, std::unique_ptr<GarbageCollector> collector);
  MemoryManager(const MemoryManager &) = delete;
  MemoryManager(MemoryManager &&);
  MemoryManager &operator=(const MemoryManager &) = delete;
  MemoryManager &operator=(MemoryManager &&);
  ~MemoryManager();

  VMValue AllocateArray(const ValueType objectType, const uint32_t length);
  void WriteToArrayIndex(const VMValue object, const void *value, const uint32_t index, const uint32_t length);
  void ReadFromArrayIndex(const VMValue object, void *value, const uint32_t index, const uint32_t length) const;
  
  bool IsArray(const VMValue object);
  ValueType GetArrayType(const VMValue object);

  void RunGc();
  uint32_t GetArrayLength(VMValue object) const;

  //
  void SetFreeSpacePointer(uint32_t value) { m_freeSpacePointer = value;  }
private:

  void EnsureFreeMemory(uint32_t requiredSpace);

  struct ArrayReadWriteData {
    uint8_t *data;
    ValueType type;
  };

  ArrayReadWriteData ArrayReadWriteCommon(const VMValue object, const uint32_t index, const uint32_t length) const;


  void EnsureNotNull(VMValue object) const;
  void EnsureArray(uint32_t type) const;
  void EnsureValidAccess(VMValue pointer) const;

  void LogMemoryLayout() const ;
  alignas(ALIGN) uint8_t *m_memory;
  uint32_t m_heapSize;
  uint32_t m_freeSpacePointer;
  std::unique_ptr<GarbageCollector> m_collector;
};


MemoryManager &MemMgrInstance();