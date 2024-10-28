#include <time.h>
#include <stdint.h>
#include "Drivers/include/keyboardDriver.h"
#include "Drivers/include/videoDriver.h"
#include "Drivers/include/SoundDriver.h"
#include "include/libasm.h"
#include <syscalls.h>
#include <naiveConsole.h>
#include "include/interrupts.h"
#include "include/scheduler.h"
#include "include/process.h"
#include "include/lib.h"
#include "include/registerHandling.h"
#include "mm/mm.h"
#include "tests/test_util.h"
#include "include/irqDispatcher.h"
#include "include/sems.h"

static void int_21();
static int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
void timer_handler();


static void (*irq_handlers[])(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

void irqDispatcher(uint64_t irq,uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    switch (irq) {
    case 0:
      timer_handler();
        break;
    case 1:
        int_21();
        break;
    case 0x80:
        int_80(rdi, rsi,  rdx, rcx, r8, r9);
        break;
    }
    return;
}

void int_21() {
    unblockProcess(1);
    keyboard_handler();
}


typedef int (*syscall_handler_t)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

static int sys_write_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    sys_write(rsi, (char *)rdx, rcx, r8);
    return 0;
}

static int sys_read_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    blockProcess(getActivePid());
    return sys_read(rsi, (char *)rdx, rcx);
}

static int clock_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    clock((char *)rsi);
    return 0;
}

static int clear_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    clear();
    return 0;
}

static int ticks_elapsed_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return ticks_elapsed();
}

static int get_height_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return getHeight();
}

static int get_width_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return getWidth();
}

static int move_cursor_x_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    moveCursorX((uint16_t)rsi);
    return 0;
}

static int move_cursor_y_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    moveCursorY((uint16_t)rsi);
    return 0;
}

static int draw_rectangle_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    drawRectangle(rsi, rdx, rcx, r8, r9);
    return 0;
}

static int sleepms_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    sleepms(rsi);
    return 0;
}

static int set_font_size_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return setFontSize(rsi);
}

static int beep_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    beep();
    return 0;
}

static int run_test_mm_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    runTestMm();
    return 0;
}

static int print_mm_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    printMm();
    return 0;
}

static int exit_process_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    exitProcess((pid_t)rsi);
    return 0;
}

static int new_process_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
 		return newProcess(rsi,rdx,rcx,r8,(char **)r9);
}

static int kill_process_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return killProcess((pid_t)rsi);
}

static int block_process_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return blockProcess((pid_t)rsi);
}

static int unblock_process_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return unblockProcess((pid_t)rsi);
}

static int alloc_memory_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return (uint64_t) allocMemory((uint64_t)rsi);
}

static int free_memory_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    freeMemory((void *)rsi);
    return 0;
}

static int change_priority_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return changePrio((pid_t)rsi, (int)rdx);
}

static int get_active_pid_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return getActivePid();
}

static int waitpid_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return waitpid((pid_t)rsi);
}

static int get_all_process_info_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return (int64_t)getAllProcessInfo();
}
static int getRegistersState(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    if(getFlag() || rsi == 1){
        if(getFlag()==0){
            drawWord(0x00FF0000,"You must take a screenshot first, press : and try again.\n");
            return 0;
        }
        printRegisters(getRegisters(), 0x00ffffff);

    }
    return 0;
}

static int yield_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    yield();
    return 0;
}
static int semOpen_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    return semOpen((char *)rsi, rdx);

}
static int semClose_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    semClose((char *)rsi);
    return 0;
}
static int semWait_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    semWait((char *)rsi);
    return 0;
}
static int semPost_wrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    semPost((char *)rsi);
    return 0;
}
static int testeandoWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    testeando();
    return 0;
}
syscall_handler_t syscallHandlers[] = {
    NULL,                       // 0 (reservado)
    sys_write_wrapper,           // 1
    sys_read_wrapper,            // 2
    clock_wrapper,               // 3
    getRegistersState,           // 4
    clear_wrapper,               // 5
    ticks_elapsed_wrapper,       // 6
    get_height_wrapper,          // 7
    get_width_wrapper,           // 8
    move_cursor_x_wrapper,       // 9
    move_cursor_y_wrapper,       // 10
    draw_rectangle_wrapper,      // 11
    sleepms_wrapper,             // 12
    set_font_size_wrapper,       // 13
    beep_wrapper,                // 14
    run_test_mm_wrapper,         // 15
    print_mm_wrapper,            // 16
    exit_process_wrapper,        // 17
    new_process_wrapper,         // 18
    kill_process_wrapper,        // 19
    block_process_wrapper,       // 20
    unblock_process_wrapper,     // 21
    alloc_memory_wrapper,        // 22
    free_memory_wrapper,         // 23
    change_priority_wrapper,     // 24
    get_active_pid_wrapper,      // 25
    testeandoWrapper,           // 26
    get_all_process_info_wrapper, // 27
    waitpid_wrapper, 				// 28
	yield_wrapper,               // 29
    semOpen_wrapper,             // 30
    semWait_wrapper,            // 31
    semPost_wrapper,            // 32
    semClose_wrapper            // 33
};

// Función int_80 utilizando el array de syscalls
static int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    if (rdi < sizeof(syscallHandlers) / sizeof(syscallHandlers[0]) && syscallHandlers[rdi] != NULL) {
        return syscallHandlers[rdi](rdi, rsi, rdx, rcx, r8, r9);
    }
    return 0;
}
