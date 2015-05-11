#pragma once
#include "VM/Memory/GarbageCollector.h"
#include "VM/Memory/RootSetProvider.h"
#include "VM/Memory/MemoryManager.h"
#include "VM/Memory/VMObjectFunctions.h"

/*
  A garbage collector based on Cheney's algorithm: http://en.wikipedia.org/wiki/Cheney%27s_algorithm
  A copying hemispace collector. Copies live objects from one hemispace (fromspace) to another hemispace (tospace) and updates references.
  After collection, fromspace and tospace are swapped.
  Dead objects are not considered, making this algorithm efficient if there is a lot of garbage
*/

class CheneyCollector : public GarbageCollector {
public:

  CheneyCollector(uint32_t heapSize, RootSetProvider *provider);
  CheneyCollector(const CheneyCollector &collector) = delete;
  CheneyCollector &operator=(const CheneyCollector &) = delete;
  ~CheneyCollector();  
  
  void Scavenge(uint8_t **memory, MemoryManager &manager) override;
  
private:
  void EvacuateRootSet(std::vector<VMValue *> rootSet, uint8_t *fromSpace);
  void EvacuateObjects(uint8_t *fromSpace);
  void CopyPointedObjects(VMValue &pointer, uint8_t *fromSpace);
  void MoveObject(VMValue *pointer, uint8_t *fromSpace);
  void PerformCopy(VMValue *pointer, uint8_t *fromSpace);

  void UpdatePointer(VMValue *pointer, uint32_t newLocation);
  void UpdateForwardingPointer(VMValue *pointer, uint8_t *fromSpace);
  uint32_t GetForwardingPointer(VMValue *pointer, uint8_t *fromSpace);


  uint32_t m_freeSpacePointer;
  
  alignas(ALIGN) uint8_t *m_toSpace;
  RootSetProvider *m_provider; // non-owning pointer. MUST NOT BE DELETED
  

};
