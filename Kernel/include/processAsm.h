#ifndef PROCESSASM_H
#define PROCESSASM_H
uint64_t createProcess(uint64_t stackBase, uint64_t rip, size_t argc, char * argv[]);
#endif