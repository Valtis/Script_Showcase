#pragma once

#include "VM/Core/VMValue.h"
#include "VM/Memory/GarbageCollector.h"
#include "VM/Memory/VMObjectFunctions.h"
#include <memory>


/*
  Memory manager class. Handles allocations and memory reads\writes. Currently only arrays are implemented.
  
  Performs necessary checks when accessing memory, such as
    * pointer is not null
    * pointer does not point into freespace
    * pointer points to array when trying to access array
    * array reads\writes are not out of bound

  Allocations are zero initialized. Uses bump-the-pointer allocation, thus no explicit free list is required.

  When allocation fails due to low memory, manager invokes the provided GC class.

*/


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

  // valid values for length is 0 - uint32_t::max. it uses signed 64 bit integer so that negative values are caught
  VMValue AllocateArray(const ValueType objectType, const int64_t length);
  void WriteToArrayIndex(const VMValue object, const void *value, const uint32_t index, const uint32_t length);
  void ReadFromArrayIndex(const VMValue object, void *value, const uint32_t index, const uint32_t length) const;
  
  bool IsArray(const VMValue object);
  ValueType GetArrayType(const VMValue object);

  void RunGc();
  uint32_t GetArrayLength(VMValue object) const;

  // used by the GC to reset free space pointer; this probably should not be public as GC is the only class that should access this.
  void SetFreeSpacePointer(uint32_t value) { m_freeSpacePointer = value;  }
private:

  void EnsureFreeMemory(uint32_t requiredSpace);

  // helper struct; used when performing operations that are shared between array reads and writes
  struct ArrayReadWriteData {
    uint8_t *data;
    ValueType type;
  };

  // shared operations between array reading and writing (such as bound checks)
  ArrayReadWriteData ArrayReadWriteShared(const VMValue object, const uint32_t index, const uint32_t length) const;


  void EnsureNotNull(VMValue object) const;
  void EnsureArray(uint32_t type) const;
  void EnsureValidAccess(VMValue pointer) const;

  void LogMemoryLayout() const ;
  alignas(ALIGN) uint8_t *m_memory;
  uint32_t m_heapSize;
  uint32_t m_freeSpacePointer;
  std::unique_ptr<GarbageCollector> m_collector;
};


// memory manager singleton. Allows global access to allocation & array reads and writes.
MemoryManager &MemMgrInstance();