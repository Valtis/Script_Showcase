#include "MemoryManager.h"
#include <string>

// 0 is reserved for null pointers and as such is invalid address. Skip first ALIGN bytes as a result
const uint32_t HEAP_BEGIN_ADDRESS = ALIGN;
const uint32_t TYPE_POINTER_SIZE = 4;
const uint32_t FORWARD_POINTER_SIZE = 4;
const uint32_t ARRAY_LENGTH_FIELD_SIZE = 4;
const uint32_t VM_NULLPTR = 0;

MemoryManager::MemoryManager(uint32_t heap_size) : m_heapSize(heap_size), m_freeSpacePointer(HEAP_BEGIN_ADDRESS) {
  m_memory = new uint8_t[heap_size];
  m_toSpace = new uint8_t[heap_size];
}

MemoryManager::MemoryManager(MemoryManager &&other) 
  : m_heapSize(other.m_heapSize), m_memory(other.m_memory), m_toSpace(other.m_toSpace), 
    m_freeSpacePointer(other.m_freeSpacePointer) {
  other.m_memory = nullptr;
  other.m_toSpace = nullptr;
}

MemoryManager &MemoryManager::operator=(MemoryManager &&rhs) {
  if (&rhs != this) {
    m_heapSize = rhs.m_heapSize;
    m_memory = rhs.m_memory;
    m_toSpace = rhs.m_toSpace;
    m_freeSpacePointer = rhs.m_freeSpacePointer;
    rhs.m_memory = nullptr;
    rhs.m_toSpace = nullptr;
  }

  return *this;
}

MemoryManager::~MemoryManager() {
  delete[] m_memory;
  delete[] m_toSpace;
}


// array layout: 
// |1 bit          |  31 bits   | 32 bits         | 32 bits      | rest       |
// |array mark bit | array type | forward pointer | array length | array data |
// so 12 byte header containing bookkeeping information and length * sizeof(array type) bytes for array itself
VMValue MemoryManager::AllocateArray(const ObjectType objectType, const uint32_t length) {
  auto requiredSpace = length*TypeSize(objectType) + FORWARD_POINTER_SIZE + TYPE_POINTER_SIZE + ARRAY_LENGTH_FIELD_SIZE;
  requiredSpace = requiredSpace + (ALIGN - requiredSpace % ALIGN) % ALIGN; // ensure alignment
  EnsureFreeMemory(requiredSpace);

  // zero-initialization of memory before handing it over
  memset(m_memory + m_freeSpacePointer, 0, requiredSpace);

  VMValue object;
  object.set_managed_pointer(m_freeSpacePointer);
  uint32_t *typeField = (uint32_t *)(m_memory + m_freeSpacePointer);
  *typeField = (1 << 31) | static_cast<uint32_t>(objectType);

  uint32_t *lengthField = (uint32_t *)(m_memory + m_freeSpacePointer + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE);
  *lengthField = length;


  m_freeSpacePointer += requiredSpace; 
  return object;
}

void MemoryManager::EnsureFreeMemory(uint32_t requiredSpace) {
  if (m_heapSize - m_freeSpacePointer < requiredSpace) {
    
    RunGc();

    if (m_heapSize - m_freeSpacePointer < requiredSpace) {
      throw std::runtime_error("Out of memory!");
    }
  }
}

void MemoryManager::WriteToArrayIndex(const VMValue object, const void *value, 
    const uint32_t index, const uint32_t length) {
  auto arrayData = ArrayReadWriteCommon(object, index, length);
  memcpy(arrayData.data + index * TypeSize(arrayData.type), value, TypeSize(arrayData.type)*length);
}

void MemoryManager::ReadFromArrayIndex(const VMValue object, void *value, 
    const uint32_t index, const uint32_t length) const {
  auto arrayData = ArrayReadWriteCommon(object, index, length);
  memcpy(value, arrayData.data + index * TypeSize(arrayData.type), TypeSize(arrayData.type)*length);
}

MemoryManager::ArrayReadWriteData MemoryManager::ArrayReadWriteCommon(const VMValue object, 
    const uint32_t index, const uint32_t readWriteLength) const {
  EnsureNotNull(object);
  uint32_t typeField = GetTypeField(object);
  EnsureArray(typeField);

  auto address = object.as_managed_pointer();
  ObjectType type = static_cast<ObjectType>(typeField & ~(1 << 31));

  auto arrayLength = GetArrayLengthUnchecked(object);

  if (index + readWriteLength > arrayLength || index < 0 || arrayLength == 0) {
    std::string err = "Out of bounds array access: Array length: " + std::to_string(arrayLength) + "   Index: "
      + std::to_string(index) + "   Access length: " + std::to_string(readWriteLength);
    throw std::runtime_error(err);
  }

  uint8_t *data = m_memory + address + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE + ARRAY_LENGTH_FIELD_SIZE;
  return { data, type };
}

void MemoryManager::EnsureNotNull(VMValue object) const {
  if (object.as_managed_pointer() == VM_NULLPTR) {
    throw std::runtime_error("Null pointer");
  }
}

uint32_t MemoryManager::GetTypeField(VMValue object) const {
  uint32_t *typeField = (uint32_t *)(m_memory + object.as_managed_pointer());
  return *typeField;
}

void MemoryManager::EnsureArray(uint32_t typeField) const {
  if ((typeField & (1 << 31)) == 0) {
    throw std::runtime_error("Non-array object provided when array object expected");
  }
}

uint32_t MemoryManager::GetArrayLength(VMValue object) const {

  uint32_t typeField = GetTypeField(object);
  EnsureArray(typeField);
  return GetArrayLengthUnchecked(object);
}

uint32_t MemoryManager::GetArrayLengthUnchecked(VMValue object) const {
  uint32_t *length_field = (uint32_t *)(m_memory + object.as_managed_pointer() 
    + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE);
  return *length_field;
}


void MemoryManager::RunGc() {
    // TODO: Implement Cheney's algorithm
}




MemoryManager &MemMgrInstance() {
  // should probably read the values from ini or something; hard coded for now
  static MemoryManager manager(1024 * 1024 * 32);
  return manager;
}