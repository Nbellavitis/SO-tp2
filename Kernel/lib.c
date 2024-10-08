// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdint.h>
#include <time.h>
#include "include/interrupts.h"
#include "Drivers/include/videoDriver.h"
#include "mm/mm.h"
#include "tests/test_util.h"
void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

void sleepms(uint32_t ms){
    uint32_t start = ticks_elapsed();
    while (ms > 18* (ticks_elapsed() - start)){
		_hlt();
	}
}
void reverseStr(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}


void intToStr(int num, char* str) {
    int i = 0;
    int isNegative = 0;

  
    if (num < 0) {
        isNegative = 1;
        num = -num; 
    }

    do {
        str[i++] = (num % 10) + '0';  
        num /= 10;                    
    } while (num != 0);

  
    if (isNegative) {
        str[i++] = '-';
    }

    
    str[i] = '\0';

 
    reverseStr(str, i);
}
void runTestMm(){
	char *argv[] = {"266240"};
    test_mm(1, argv);
}
void printMm(){
	         char status[50];
            MemoryStatus mmStatus = getMemoryStatus();
            drawWord(0x00ffffff,"Memoria Total:");
            intToStr(mmStatus.totalMemory,status);
            drawWord(0x00ffffff,status);
            drawChar(0x00ffffff,'\n');
            drawWord(0x00ffffff,"Memoria usada:");
             intToStr(mmStatus.usedMemory,status);
            drawWord(0x00ffffff,status);
            drawChar(0x00ffffff,'\n');
            drawWord(0x00ffffff,"Memoria libre:");
            intToStr(mmStatus.freeMemory,status);
            drawWord(0x00ffffff,status);
            drawChar(0x00ffffff,'\n');
}
void testeando(){
    drawWord(0xFFFFFFFF,"hola");
}
