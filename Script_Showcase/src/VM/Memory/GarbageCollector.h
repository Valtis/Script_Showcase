#pragma once
#include <cstdint>
class MemoryManager;
class GarbageCollector {
public:
  GarbageCollector();
  virtual ~GarbageCollector();
  GarbageCollector(const GarbageCollector &collector) = delete;
  GarbageCollector &operator=(const GarbageCollector &) = delete;
  virtual void Scavenge(uint8_t **memory, MemoryManager &manager) = 0;
private:

};