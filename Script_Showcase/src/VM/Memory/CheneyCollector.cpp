#include "VM/Memory/CheneyCollector.h"
#include "Utility/LoggerManager.h"
#include <cassert>

CheneyCollector::CheneyCollector(uint32_t heapSize, RootSetProvider *provider) : m_provider(provider) {
  m_toSpace = new uint8_t[heapSize];
}

CheneyCollector::~CheneyCollector() {
  delete[] m_toSpace;
}

void CheneyCollector::Scavenge(uint8_t **fromSpace, MemoryManager &manager) {
  if (m_provider == nullptr) {
    return;
  }
  
  std::vector<VMValue *> rootSet = m_provider->GetRootSet();

  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO, "Root set size: " + std::to_string(rootSet.size()) + " objects");

  EvacuateRootSet(rootSet, *fromSpace);
  EvacuateObjects(*fromSpace);
  std::swap(m_toSpace, *fromSpace);
  manager.SetFreeSpacePointer(m_freeSpacePointer);
}

// evacuates root set from fromSpace to toSPace
void CheneyCollector::EvacuateRootSet(std::vector<VMValue *> rootSet, uint8_t *fromSpace) {
  m_freeSpacePointer = HEAP_BEGIN_ADDRESS;
  for (auto &value : rootSet) {
    MoveObject(value, fromSpace);
  }
}

// scans objects in toSpace, searching for pointers, and copies the objects from fromSpace to toSpace (if not copied yet), and updates the pointer
void CheneyCollector::EvacuateObjects(uint8_t *fromSpace) {
  auto scanPointer = HEAP_BEGIN_ADDRESS;

  while (scanPointer < m_freeSpacePointer) {
    VMValue pointer;
    pointer.SetManagedPointer(scanPointer);
    CopyPointedObjects(pointer, fromSpace);
    scanPointer += VMObjectFunction::CalculateObjectSize(pointer, m_toSpace);
  }
}

// scans object, which is in toSpace, pointed by VMValue &pointer, for sub-pointers. Then copies the objects pointed by subpointer (if needed)
// and updates the subpointer
void CheneyCollector::CopyPointedObjects(VMValue &pointer, uint8_t *fromSpace) {
  
  assert(pointer.AsManagedPointer() != VM_NULLPTR);
  auto typeField = VMObjectFunction::GetTypeField(pointer, m_toSpace);
  
  if (VMObjectFunction::IsArray(typeField)) {
    // if array, check the type. If the array is not an array of managed pointer (basically Object []), stop
    if (VMObjectFunction::GetArrayValueType(typeField) != ValueType::MANAGED_POINTER) {
      return;
    }


    // some calculate starting position, get array length, get index size
    auto position = pointer.AsManagedPointer() + VMObjectFunction::ArrayMetaDataSize();
    auto length = VMObjectFunction::GetArrayLengthUnchecked(pointer, m_toSpace);
    auto typeSize = TypeSize(VMObjectFunction::GetArrayValueType(typeField));


    for (size_t i = 0; i < length; ++i) {
      uint32_t pointerAddress;

      // copy the pointer address into pointerAddress variable
      auto pointerInArrayLocation = position + i*typeSize;
      memcpy(&pointerAddress, m_toSpace + pointerInArrayLocation, sizeof(pointerAddress));

      // temp conversion, MoveObject expects VmValue
      VMValue tempPointer;
      tempPointer.SetManagedPointer(pointerAddress);
      // move object (if needed) that is in the fromspace, update pointer
      MoveObject(&tempPointer, fromSpace);

      // write new address of the object back to array index
      pointerAddress = tempPointer.AsManagedPointer();
      memcpy(m_toSpace + pointerInArrayLocation, &pointerAddress, sizeof(pointerAddress));
    }
  }
  else {
    // regular objects have not been implemented
    throw std::logic_error("Copying not implemented for non-arrays");
  }
}

// moves object pointed by the pointer if it did not contain forwarding pointer, otherwise merely updates pointer
void CheneyCollector::MoveObject(VMValue *pointer, uint8_t *fromSpace) {
  if (pointer->AsManagedPointer() == VM_NULLPTR) {
    return;
  }

  auto forwardingPointer = GetForwardingPointer(pointer, fromSpace);
  if (forwardingPointer != 0) {
    UpdatePointer(pointer, forwardingPointer);
    return;
  }
  // perform actual copy since object wasn't moved yet
  PerformCopy(pointer, fromSpace);
}

void CheneyCollector::PerformCopy(VMValue *pointer, uint8_t *fromSpace) {

  // get object size, and copy it from fromSpace into toSpace
  auto size = VMObjectFunction::CalculateObjectSize(*pointer, fromSpace);
  memcpy(m_toSpace + m_freeSpacePointer, fromSpace + pointer->AsManagedPointer(), size);

  // write new object address into the forwarding pointer
  UpdateForwardingPointer(pointer, fromSpace);
  // and update the pointer to point to new location in toSpace
  UpdatePointer(pointer, m_freeSpacePointer);
  // update free space pointer
  m_freeSpacePointer += size;
}

// writes new object address into the forwarding pointer field in the old object location in fromSpace
void CheneyCollector::UpdateForwardingPointer(VMValue *pointer, uint8_t *fromSpace) {
  auto position = pointer->AsManagedPointer() + TYPE_POINTER_SIZE; // skip first header field
  memcpy(fromSpace + position, &m_freeSpacePointer, sizeof(m_freeSpacePointer));
}


uint32_t CheneyCollector::GetForwardingPointer(VMValue *pointer, uint8_t *fromSpace) {
  uint32_t ptr;
  auto position = pointer->AsManagedPointer() + TYPE_POINTER_SIZE; // skip first header field
  memcpy(&ptr, fromSpace + position, sizeof(FORWARD_POINTER_SIZE));
  return ptr;
}

void CheneyCollector::UpdatePointer(VMValue *pointer, uint32_t newValue) {
  VMValue newLocation;
  newLocation.SetManagedPointer(newValue);
  *pointer = newLocation;
}
