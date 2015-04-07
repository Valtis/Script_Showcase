#pragma once

#include <map>
#include "custom_allocator.h"

enum class AllocationType : int { REGULAR, ARRAY } ;


/* Class that is used to track any allocations made. Instrumented new\delete-functions use this */
class allocations {
public:

  void add_allocation(const void *addr, const AllocationType type, const size_t bytes);
  void remove_allocation(const void *addr, const AllocationType type);

  bool has_errors() const;
  void print_errors() const;

  void clear();

private:

  // two maps so we can differentiate between new, new[], delete and delete[]
  std::map<const void *,
           size_t,
           std::less<const void *>,
           custom_allocator<std::pair<const void *, size_t>>> m_regular_allocations;
  std::map<const void *,
           size_t,
           std::less<const void *>,
           custom_allocator<std::pair<const void *, size_t>>> m_array_allocations;

  // catch double deletes
  std::map<const void *,
           uint32_t,
           std::less<const void *>,
           custom_allocator<std::pair<const void *, uint32_t>>> m_regular_deletes_without_matching_allocations;
  std::map<const void *,
           uint32_t,
           std::less<const void *>,
           custom_allocator<std::pair<const void *, uint32_t>>> m_array_deletes_without_matching_allocations;

};
