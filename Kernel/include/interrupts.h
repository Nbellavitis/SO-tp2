 /*
 *   interrupts.h
 *
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include <idtLoader.h>

#include <stdint.h>
void acquire(uint8_t *lock);
void release(uint8_t *lock);
void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);
void _irq80Handler(void);
void _hlt(void);
void _exception0Handler(void);
void _exception6Handler(void);
void _cli(void);
void saveRegisters();
 uint64_t * getRegisters();
short getFlag();
void printRegistersAsm(uint32_t colour);

void _sti(void);

void _hlt(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

void yield(void);

//Termina la ejecución de la cpu.
void haltcpu(void);

#endif /* INTERRUPS_H_ */
