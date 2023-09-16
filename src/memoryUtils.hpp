//
// Created by emma on 15/09/23.
//

#include <iostream>

struct AllocationMetric {
  uint32_t TotalAllocated {0};
  uint32_t TotalFreed {0};

  uint32_t CurrentUsage() {
    return TotalAllocated - TotalFreed;
  }
};

static AllocationMetric s_AllocationMetric;

void* operator new(size_t size) {
  s_AllocationMetric.TotalAllocated += size;

  return malloc(size);
}

void operator delete(void* memory, size_t size) {
  s_AllocationMetric.TotalFreed += size;

  free(memory);
}

static void PrintMemoryUsage() {
  std::cout << "Memory Usage: " << s_AllocationMetric.CurrentUsage() << " bytes\n";
}