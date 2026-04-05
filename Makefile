PREFIX := $(HOME)/opt/cross
TARGET := i686-elf
PATH := $(PREFIX)/bin:$(PATH)

CROSS_GCC := $(PREFIX)/bin/$(TARGET)-gcc
CROSS_LD := $(PREFIX)/bin/$(TARGET)-ld
NASM := nasm

ifeq ($(shell test -f $(CROSS_GCC) && echo yes),yes)
CC := $(CROSS_GCC)
CFLAGS := -ffreestanding -fno-pic -fno-pie -c
LDFLAGS := -Ttext 0x1000 --oformat binary
else
CC := gcc
CFLAGS := -m32 -ffreestanding -fno-pic -fno-pie -c
LDFLAGS := -m elf_i386 -Ttext 0x1000 --oformat binary
endif

ifeq ($(shell test -f $(CROSS_LD) && echo yes),yes)
LD := $(CROSS_LD)
else
LD := ld
endif

all: bin/os.bin
	qemu-system-x86_64 -drive format=raw,file=bin/os.bin

bin/os.bin: bin/bootloader.bin bin/kernel.bin
	dd if=/dev/zero of=bin/os.bin bs=512 count=2880
	dd if=bin/bootloader.bin of=bin/os.bin conv=notrunc
	dd if=bin/kernel.bin of=bin/os.bin bs=512 seek=1 conv=notrunc

bin/bootloader.bin: src/bootloader.asm
	$(NASM) -f bin -I src/ src/bootloader.asm -o bin/bootloader.bin

bin/kernel.bin: src/kernel/kernel.c src/kernel/enter_kernel.asm
	$(NASM) src/kernel/enter_kernel.asm -f elf32 -o build/enter_kernel.o
	$(CC) $(CFLAGS) src/kernel/kernel.c -o build/kernel.o
	$(LD) $(LDFLAGS) build/enter_kernel.o build/kernel.o -o bin/kernel.bin


clean:
	rm -rf bin/* build/*
