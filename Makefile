

all: toolchain bootloader kernel userland image

toolchain:
	cd Toolchain; make all

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all MM=$(MM)

userland:
	cd Userland; make all

image: kernel bootloader toolchain userland
	cd Image; make all

# Compilar los archivos .c de Kernel/tests, Kernel/mm y externApp solamente cuando se llame a externAppBuild
externAppBuild: cleanExecutable externAppExecutable cleanObjects

externAppExecutable:
	
	rm -f $(wildcard Kernel/tests/*.o) $(wildcard Kernel/mm/*.o) $(wildcard externApp/*.o)

	$(CC) -o externAppExecutable $(wildcard Kernel/mm/*.c) $(wildcard externApp/*.c) $(CFLAGS) $(LDFLAGS)


# Limpiar solo los archivos objeto relacionados con externApp
cleanObjects:
	rm -f  $(wildcard Kernel/mm/*.o) $(wildcard externApp/*.o)

cleanExecutable:
	rm -f  $(wildcard externAppExecutable)
clean:
	cd Toolchain; make clean
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean
	rm -f externAppExecutable

.PHONY: toolchain bootloader image collections kernel userland externAppBuild all clean
