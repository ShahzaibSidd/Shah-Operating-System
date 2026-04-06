PREFIX := $(HOME)/opt/cross
TARGET := i686-elf
PATH := $(PREFIX)/bin:$(PATH)

CROSS_GCC := $(PREFIX)/bin/$(TARGET)-gcc
CROSS_LD := $(PREFIX)/bin/$(TARGET)-ld
NASM := nasm

ifeq ($(shell test -f $(CROSS_GCC) && echo yes),yes)
CC := $(CROSS_GCC)
CFLAGS := -ffreestanding -fno-pic -fno-pie -c -g
LDFLAGS := -Ttext 0x1000 --oformat binary
else
CC := gcc
CFLAGS := -m32 -ffreestanding -fno-pic -fno-pie -c -g
LDFLAGS := -m elf_i386 -Ttext 0x1000 --oformat binary
endif

ifeq ($(shell test -f $(CROSS_LD) && echo yes),yes)
LD := $(CROSS_LD)
else
LD := ld
endif

ifeq ($(shell test -f $(CROSS_LD) && echo yes),yes)
LDFLAGS_ELF := -Ttext 0x1000
else
LDFLAGS_ELF := -m elf_i386 -Ttext 0x1000
endif

C_SOURCES := $(wildcard src/kernel/*.c src/drivers/*.c)
OBJ := $(addprefix build/, $(notdir $(C_SOURCES:.c=.o)))

all: bin/os.bin
	qemu-system-i386 -drive format=raw,file=$<

bin/os.bin: bin/bootloader.bin bin/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=bin/bootloader.bin of=$@ conv=notrunc
	dd if=bin/kernel.bin of=$@ bs=512 seek=1 conv=notrunc

bin/bootloader.bin: src/boot/bootloader.asm
	$(NASM) -f bin -I src/ $< -o $@

bin/kernel.bin: build/enter_kernel.o $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

build/enter_kernel.o: src/kernel/enter_kernel.asm
	$(NASM) $< -f elf32 -o $@

build/%.o: src/*/%.c
	$(CC) $(CFLAGS) $< -o $@


bin/kernel.elf: build/enter_kernel.o $(OBJ)
	$(LD) $(LDFLAGS_ELF) $^ -o $@

gdb: bin/os.bin bin/kernel.elf
	qemu-system-i386 -drive format=raw,file=bin/os.bin -s -S &
	gdb bin/kernel.elf \
		-ex "set architecture i386" \
		-ex "target remote :1234" \
		-ex "break main" \
		-ex "continue"

clean:
	rm -rf bin/* build/*