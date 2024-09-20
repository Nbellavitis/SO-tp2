include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm
SOURCES_TESTS= $(wildcard tests/*.c)
OBJECTS_TESTS = $(SOURCES_TESTS:.c=.o)
SOURCES_BITMAP_MM = $(wildcard mm/mm.c)
OBJECTS_BITMAP_MM = $(SOURCES_BITMAP_MM:.c=.o)
SOURCES_DRIVERS=$(wildcard Drivers/*.c)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_BITMAP_MM) $(OBJECTS_TESTS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_DRIVERS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_BITMAP_MM) $(OBJECTS_TESTS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin
	rm -rf drivers/*.o *.o *.bin
	rm -rf /*.o *.o *.bin

.PHONY: all clean
