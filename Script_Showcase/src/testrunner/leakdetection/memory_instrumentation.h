#pragma once
#include "allocations.h"


// returns pointer to class that tracks allocations made. Instance is singleton 
allocations *get_allocations_list();
