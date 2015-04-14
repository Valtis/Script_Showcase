#pragma once
#include "VM/Memory/GarbageCollector.h"
#include "VM/Memory/RootSetProvider.h"
#include <memory>
enum class CollectorType {
  CHENEY_COLLECTOR
};

namespace GCFactory {
  std::unique_ptr<GarbageCollector> GetGC(CollectorType type, RootSetProvider *provider, uint32_t heapSize);
}