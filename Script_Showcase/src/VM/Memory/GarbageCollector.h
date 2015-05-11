#pragma once
#include <cstdint>

/*
  Interface for garbage collector. The memorymanager uses this interface, thus hiding the actual algorithm from the manager.
  All GC algorithms must implement this interface.
*/


class MemoryManager;
class GarbageCollector {
public:
  GarbageCollector();
  virtual ~GarbageCollector();
  GarbageCollector(const GarbageCollector &collector) = delete;
  GarbageCollector &operator=(const GarbageCollector &) = delete;
  // memory is provided as pointer to the byte-array (thus, double pointer), in case the GC algorithm needs to swap the actual memory buffer
  // used by the memory manager
  virtual void Scavenge(uint8_t **memory, MemoryManager &manager) = 0;
private:

};