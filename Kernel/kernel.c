// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "Drivers/include/keyboardDriver.h"
#include "Drivers/include/videoDriver.h"
#include "collections/hashMap.h"
#include "collections/queue.h"
#include "include/lib.h"
#include "include/pipe.h"
#include "include/process.h"
#include "include/scheduler.h"
#include "include/sems.h"
#include "mm/mm.h"
#include <idtLoader.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;
static void *const heapAddress = (void *)0x600000;
typedef int (*EntryPoint)();
extern void initMap();
void clearBSS(void *bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
  return (void *)((uint64_t)&endOfKernel +
                  PageSize * 8       // The size of the stack itself, 32KiB
                  - sizeof(uint64_t) // Begin at the top of the stack
  );
}

void *retUserland() { return sampleCodeModuleAddress; }

void *initializeKernelBinary() {
  char buffer[10];

  ncPrint("[x64BareBones]");
  ncNewline();

  ncPrint("CPU Vendor:");
  ncPrint(cpuVendor(buffer));
  ncNewline();

  ncPrint("[Loading modules]");
  ncNewline();
  void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

  loadModules(&endOfKernelBinary, moduleAddresses);
  ncPrint("[Done]");
  ncNewline();
  ncNewline();

  ncPrint("[Initializing kernel's binary]");
  ncNewline();

  clearBSS(&bss, &endOfKernel - &bss);

  ncPrint("  text: 0x");
  ncPrintHex((uint64_t)&text);
  ncNewline();
  ncPrint("  rodata: 0x");
  ncPrintHex((uint64_t)&rodata);
  ncNewline();
  ncPrint("  data: 0x");
  ncPrintHex((uint64_t)&data);
  ncNewline();
  ncPrint("  bss: 0x");
  ncPrintHex((uint64_t)&bss);
  ncNewline();

  ncPrint("[Done]");
  ncNewline();
  ncNewline();

  return getStackBase();
}

int initShell() {
  char *fds[2] = {"tty", "tty"};
  char *argv[] = {"shell", "null"};
  newProcess((uint64_t)sampleCodeModuleAddress, 1, 1, 1, argv, fds);
  return 1;
}

int main() {
  loadIdt();
  mmInit(heapAddress, 0x2700000);
  initMap();
  startScheduler();
  semInit();
  initPipes();
  semOpen("STDIN", 0);
  initShell();

  while (1) {
  }
}
