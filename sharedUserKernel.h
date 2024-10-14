//
// Created by nicol on 10/14/2024.
//

#ifndef SO_SHAREDUSERKERNEL_H
#define SO_SHAREDUSERKERNEL_H
typedef int64_t pid_t;
typedef struct processInfo {
    pid_t pid;
    pid_t ppid;
    uint64_t rsp;
    uint64_t stackBase;
    uint64_t rip;
    uint64_t ground;
    uint64_t priority;
    uint64_t status;
    char * name;
} processInfo;

typedef struct processInfo * processInfoPtr;

#endif //SO_SHAREDUSERKERNEL_H
