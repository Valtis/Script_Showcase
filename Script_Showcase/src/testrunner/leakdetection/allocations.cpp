#include "allocations.h"
#include <iostream>


// registers allocation to address with the size of allocation
void allocations::add_allocation(const void * addr, const AllocationType type, const size_t bytes) {

  switch (type) {
    case AllocationType::REGULAR:
      m_regular_allocations[addr] = bytes;
    break;

    case AllocationType::ARRAY:
      m_array_allocations[addr] = bytes;
    break;
  }

}
// removes allocation from allocations list, or registers invalid call to delete if no such address
// has been registered before
void allocations::remove_allocation(const void * addr, const AllocationType type) {
  switch (type) {
    case AllocationType::REGULAR:
    if (m_regular_allocations.count(addr)) {
      m_regular_allocations.erase(addr);
    } else if (addr != nullptr) {
      m_regular_deletes_without_matching_allocations[addr] += 1;
    }
    break;

    case AllocationType::ARRAY:
    if (m_array_allocations.count(addr)) {
      m_array_allocations.erase(addr);
    } else if (addr != nullptr) {
      m_array_deletes_without_matching_allocations[addr] += 1;
    }
    break;
  }
}

// returns true if some new calls did not have matching delete call, or if some deletes did not have
// matching new call
bool allocations::has_errors() const {
  return m_regular_allocations.size() != 0 || m_array_allocations.size() != 0
    || m_regular_deletes_without_matching_allocations.size() != 0
    || m_array_deletes_without_matching_allocations.size() != 0;
}

void allocations::print_errors() const {
  for (auto pair : m_regular_allocations) {
    std::cout << "Memory leak at address " << pair.first << ". " << pair.second << " bytes lost\n";
  }

  for (auto pair : m_array_allocations) {
    std::cout << "Array memory leak at address " << pair.first << ". " << pair.second << " bytes lost\n";
  }

  for (auto pair : m_regular_deletes_without_matching_allocations) {
    std::cout << "Delete called " << pair.second << " times with address " << pair.first << " without matching new call\n";
  }

  for (auto pair : m_array_deletes_without_matching_allocations) {
    std::cout << "Array delete called " << pair.second << " times with address " << pair.first << " without matching new[] call\n";
  }
}

void allocations::clear() {
  m_regular_allocations.clear();
  m_array_allocations.clear();
  m_regular_deletes_without_matching_allocations.clear();
  m_array_deletes_without_matching_allocations.clear();
}
