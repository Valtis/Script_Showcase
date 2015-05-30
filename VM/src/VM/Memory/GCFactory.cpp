#include "VM/Memory/GCFactory.h"
#include "VM/Memory/CheneyCollector.h"


namespace GCFactory {
  std::unique_ptr<GarbageCollector> GetGC(CollectorType type, RootSetProvider *provider, uint32_t heapSize) {
    switch (type) {
    case CollectorType::CHENEY_COLLECTOR:
      return std::unique_ptr<GarbageCollector>{ new CheneyCollector(heapSize, provider) };
    default:
      throw std::runtime_error("Invalid garbage collector type specified");
    }
  }
}