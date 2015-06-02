#include "VM/Core/VM.h"
#include "VM/Exceptions/Exceptions.h"
#include "VM/Memory/MemoryManager.h"
#include "VM/Memory/GCFactory.h"
#include "Utility/LoggerManager.h"
#include <string>
#ifdef _MSC_VER
// for performance counter
#include <windows.h>
#undef ERROR // windows.h really likes to define random stuff
#endif 

// 0 is reserved for null pointers and as such is invalid address. Skip first ALIGN bytes as a result



MemoryManager::MemoryManager(uint32_t heap_size, std::unique_ptr<GarbageCollector> collector) 
  : m_heapSize(heap_size), m_freeSpacePointer(HEAP_BEGIN_ADDRESS), m_collector(std::move(collector)) {
  m_memory = new uint8_t[heap_size];
}

MemoryManager::MemoryManager(MemoryManager &&other) 
  : m_memory(other.m_memory), m_heapSize(other.m_heapSize), 
  m_freeSpacePointer(other.m_freeSpacePointer), m_collector(std::move(other.m_collector)) {
  other.m_memory = nullptr;
}

MemoryManager &MemoryManager::operator=(MemoryManager &&rhs) {
  if (&rhs != this) {
    m_heapSize = rhs.m_heapSize;
    m_memory = rhs.m_memory;
    m_freeSpacePointer = rhs.m_freeSpacePointer;
    m_collector = std::move(rhs.m_collector);
    rhs.m_memory = nullptr;
    
  }

  return *this;
}

MemoryManager::~MemoryManager() {
  delete[] m_memory;
}


// array layout: 
// |1 bit          |  31 bits   | 32 bits         | 32 bits      | rest       |
// |array mark bit | array type | forward pointer | array length | array data |
// so 12 byte header containing bookkeeping information and length * sizeof(array type) bytes for array itself
VMValue MemoryManager::AllocateArray(const ValueType objectType, const int64_t length) {
  if (length < 0) {
    throw InvalidArrayLengthError(std::string("Array length cannot be negative (") + std::to_string(length) + ")");
  } 
  
  if (length > UINT32_MAX) {
    throw InvalidArrayLengthError(std::string("Array length too large: Max is ") + std::to_string(UINT32_MAX) + " but was " + std::to_string(length));
  }

  uint64_t requiredSpace = static_cast<uint32_t>(length)*TypeSize(objectType) + VMObjectFunction::ArrayHeaderSize();
  if (requiredSpace > UINT32_MAX) {
    throw RequiredHeapSpaceTooLarge("Allocation of " + std::to_string(requiredSpace) 
      + " bytes requested but " + std::to_string(UINT32_MAX) + " is maximum allowed allocation size");
  }
  // align the memory (for example, if aligning to 4 bytes, 14 bytes becomes 16 bytes). Reduces unaligned memory accesses
  // which can potentially be slow (or cause 
  requiredSpace = VMObjectFunction::AlignSize(requiredSpace);
  
  EnsureFreeMemory(static_cast<uint32_t>(requiredSpace));

  // zero-initialization of memory before handing it over
  memset(m_memory + m_freeSpacePointer, 0, static_cast<uint32_t>(requiredSpace));

  VMValue object;
  object.SetManagedPointer(m_freeSpacePointer);

  // get the address to the typefield, and interpret it as uint32_t pointer, thus allowing us to write 32 bit values to the address 
  // when dereferencing the pointer
  auto typeField = reinterpret_cast<uint32_t *>(m_memory + m_freeSpacePointer);
  *typeField = (1 << 31) | static_cast<uint32_t>(objectType); // write array mark bit & type into first 4 bytes of header

  // same operation as above, but now writing to array length field.
  auto lengthField = reinterpret_cast<uint32_t *>(m_memory + m_freeSpacePointer + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE);
  *lengthField = static_cast<uint32_t>(length);

  // bump the free space pointer
  m_freeSpacePointer += static_cast<uint32_t>(requiredSpace); 
  return object;
}

/*
  Checks if there is enough free memory to satisfy the allocation. If not, invokes GC. If there is no enough free memory after gc, throws an exception.
*/
void MemoryManager::EnsureFreeMemory(uint32_t requiredSpace) {
  if (m_heapSize - m_freeSpacePointer < requiredSpace) {
    LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO, "Allocation failure due to low memory, initiating GC");
    RunGc();

    if (m_heapSize - m_freeSpacePointer < requiredSpace) {

      LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::ERROR, "Failed to allocate memory after GC. Out of memory");
      throw std::runtime_error("Out of memory!");
    }
  }
}

void MemoryManager::WriteToArrayIndex(const VMValue object, const void *value, 
    const uint32_t index, const uint32_t length) {
  if (length == 0) {
    return;
  }
  auto arrayData = ArrayReadWriteShared(object, index, length);
  memcpy(arrayData.data + index * TypeSize(arrayData.type), value, TypeSize(arrayData.type)*length);
}

void MemoryManager::ReadFromArrayIndex(const VMValue object, void *value, 
    const uint32_t index, const uint32_t length) const {
  if (length == 0) {
    return;
  }
  auto arrayData = ArrayReadWriteShared(object, index, length);
  memcpy(value, arrayData.data + index * TypeSize(arrayData.type), TypeSize(arrayData.type)*length);
}

bool MemoryManager::IsArray(const VMValue object) {
  if (object.GetType() != ValueType::MANAGED_POINTER) {
    return false;
  }
  
  auto typeField = VMObjectFunction::GetTypeField(object, m_memory);
  return VMObjectFunction::IsArray(typeField);
}

ValueType MemoryManager::GetArrayType(const VMValue object) {
  EnsureNotNull(object);
  uint32_t typeField = VMObjectFunction::GetTypeField(object, m_memory);
  EnsureArray(typeField);
  return VMObjectFunction::GetArrayValueType(typeField);
}

// checks that pointer is not null, points to array, pointer is within allocated memory area and that index & access length are within the array
MemoryManager::ArrayReadWriteData MemoryManager::ArrayReadWriteShared(const VMValue object, 
    const uint32_t index, const uint32_t readWriteLength) const {
  EnsureNotNull(object);

  
  uint32_t typeField = VMObjectFunction::GetTypeField(object, m_memory);
  EnsureArray(typeField);

  auto address = object.AsManagedPointer();
  ValueType type = VMObjectFunction::GetArrayValueType(typeField);

  auto arrayLength = VMObjectFunction::GetArrayLengthUnchecked(object, m_memory);


  // bound check. As we use unsigned integers, negative values become large positive values. Thus we need only check for overflows
  if (index > arrayLength - readWriteLength || arrayLength == 0) {
    std::string err = "Out of bounds array access: Array length: " + std::to_string(arrayLength) + "   Index: "
      + std::to_string(index) + "   Access length: " + std::to_string(readWriteLength);
    throw ArrayIndexOutOfBoundsError(err);
  }

  // pointer to the beginning of data segment. Skips the header
  uint8_t *data = m_memory + address + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE + ARRAY_LENGTH_FIELD_SIZE;
  return { data, type };
}


void MemoryManager::EnsureNotNull(VMValue object) const {
  if (object.AsManagedPointer() == VM_NULLPTR) {
    throw std::runtime_error("Null pointer");
  }
}


void MemoryManager::EnsureArray(uint32_t typeField) const {
  if (VMObjectFunction::IsArray(typeField) == false) {
    throw std::runtime_error("Non-array object provided when array object expected");
  }
}

void MemoryManager::EnsureValidAccess(VMValue pointer) const{
  if (pointer.AsManagedPointer() >= m_freeSpacePointer) {
    throw std::runtime_error("Read beyond allocated memory");
  }
}

uint32_t MemoryManager::GetArrayLength(VMValue object) const {

  uint32_t typeField = VMObjectFunction::GetTypeField(object, m_memory);
  EnsureNotNull(object);
  EnsureArray(typeField);
  EnsureValidAccess(object);
  return VMObjectFunction::GetArrayLengthUnchecked(object, m_memory);
}


// invokes GC
void MemoryManager::RunGc() {
  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO,
    "Initiating GC with " + std::to_string(m_freeSpacePointer) + " bytes in use");

  // Windows specific performance counters.
#ifdef _MSC_VER
  LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
  LARGE_INTEGER Frequency;

  QueryPerformanceFrequency(&Frequency);
  QueryPerformanceCounter(&StartingTime);
#endif
  
  if (m_collector != nullptr) {
    m_collector->Scavenge(&m_memory, *this);
  }  else {
    LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::ERROR, "No garbage collector set - skipping collection");
  }


#ifdef _MSC_VER   
  QueryPerformanceCounter(&EndingTime);
  ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
  ElapsedMicroseconds.QuadPart *= 1000000;
  ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

#endif

  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO,
    "GC finished. " + std::to_string(m_freeSpacePointer) + " bytes in use."
#ifdef _MSC_VER 
    + " Time spent (microseconds) : " + std::to_string(ElapsedMicroseconds.QuadPart)
#endif
    );
}


// debug function. Writes the memory layout into log file. 
void MemoryManager::LogMemoryLayout() const {
  std::string msg = "Memory layout dump:\n";
  for (size_t i = HEAP_BEGIN_ADDRESS; i < m_freeSpacePointer;) {
    VMValue f;
    f.SetManagedPointer(i);
    auto typeField = VMObjectFunction::GetTypeField(f, m_memory);
    if (VMObjectFunction::IsArray(typeField)) {
      msg += "\nArray at address " + std::to_string(i) + "\n";
      msg += "Type tag " + TypeToString(VMObjectFunction::GetArrayValueType(typeField)) + " (Size: " + 
        std::to_string(TypeSize(VMObjectFunction::GetArrayValueType(typeField))) + ")\n";
      msg += "Length: " + std::to_string(VMObjectFunction::GetArrayLengthUnchecked(f, m_memory)) + "\n";

    }
    else {
      msg += "\nInvalid object in memory dump - terminating dump\n";
      break;
    }

    i += VMObjectFunction::CalculateObjectSize(f, m_memory);
  }


  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::DEBUG, msg);
}


// memory manager singleton. Currently hardcoded to provide 32 megabyte heap (thus total memory required is 64 megabytes due to usage of hemispace collector)
MemoryManager &MemMgrInstance() {
  // should probably read the values from ini or something; hard coded for now
  auto size = 1024 * 1024 * 32;
  static MemoryManager manager(size, GCFactory::GetGC(CollectorType::CHENEY_COLLECTOR, &VMInstance(), size));
  return manager;
}