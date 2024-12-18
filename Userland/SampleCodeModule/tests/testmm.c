// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "testUtil.h"
#define MAX_BLOCKS 128
typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

void testMm(int argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc < 1) {
    print(0x00ff0000, "ERROR argc\n");
    return;
  }

  if ((max_memory = satoi(argv[1])) <= 0) {
    print(0x00ff0000, "ERROR max_memory\n");
    return;
  }

  while (1) {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = allocMemory(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          print(0x00ff0000, "ERROR TEST_MM\n");
          return;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        freeMemory(mm_rqs[i].address);
  }
}