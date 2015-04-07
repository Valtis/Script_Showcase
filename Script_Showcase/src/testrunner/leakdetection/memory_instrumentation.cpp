#include "memory_instrumentation.h"
/*
  global override new\delete. This allows us to instrument all calls and see if we have memory leaks
  As it turns out, defining new\delete and their [] variants in any translation unit overrides
  the usual new\delete operators. This allows us to track any and all allocations made during
  execution of the tests. As all memory should have been released when the test function exits, we
  easily see if there are any leaks by checking if delete was not called to new'ed pointers.
*/
void *operator new(std::size_t n) {
  auto alloc = get_allocations_list();
  auto ptr =  malloc(n);

  if (ptr == nullptr) {
    throw std::bad_alloc{};
  }

  alloc->add_allocation(ptr, AllocationType::REGULAR, n);
  return ptr;
}

void *operator new[](std::size_t n) {
  auto alloc = get_allocations_list();
  auto ptr =  malloc(n);
  if (ptr == nullptr) {
    throw std::bad_alloc{};
  }

  alloc->add_allocation(ptr, AllocationType::ARRAY, n);
  return ptr;

}


void operator delete(void * p) {
  auto alloc = get_allocations_list();
  alloc->remove_allocation(p, AllocationType::REGULAR);
  free(p);
}


void operator delete[](void * p) {
  auto alloc = get_allocations_list();
  alloc->remove_allocation(p, AllocationType::ARRAY);

  free(p);
}


allocations *get_allocations_list() {
  static allocations allocs;
  return &allocs;
}
