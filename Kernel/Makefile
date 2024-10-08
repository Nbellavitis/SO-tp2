include Makefile.inc

KERNEL=kernel.bin
KERNEL_ELF=kernel.elf
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
SOURCES_LIB = $(wildcard lib/*.c)
OBJECTS_LIB = $(SOURCES_LIB:.c=.o)
SOURCES_QUEUE = $(wildcard collections/*.c)
OBJECTS_QUEUE = $(SOURCES_QUEUE:.c=.o) 

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

# La regla "all" ahora generará tanto el ELF como el archivo binario.
all: $(KERNEL) $(KERNEL_ELF)

# Regla para generar el archivo binario (igual que antes)
$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_BITMAP_MM) $(OBJECTS_TESTS) $(OBJECTS_QUEUE) $(OBJECTS_LIB)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_DRIVERS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_BITMAP_MM) $(OBJECTS_TESTS) $(OBJECTS_QUEUE) $(OBJECTS_LIB)

# Nueva regla para generar el archivo ELF
$(KERNEL_ELF): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_BITMAP_MM) $(OBJECTS_TESTS)  $(OBJECTS_QUEUE) $(OBJECTS_LIB)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL_ELF) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_DRIVERS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_BITMAP_MM) $(OBJECTS_TESTS)  $(OBJECTS_QUEUE) $(OBJECTS_LIB)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin *.elf
	rm -rf drivers/*.o *.o *.bin *.elf
	rm -rf /*.o *.o *.bin *.elf

.PHONY: all clean
