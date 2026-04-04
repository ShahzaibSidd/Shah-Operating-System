all:
	nasm -f bin -I src/ src/bootloader.asm -o bin/bootloader.bin

	gcc -ffreestanding -c src/kernel.c -o build/kernel.o
	ld -o bin/kernel.bin -Ttext 0x1000 build/kernel.o --oformat binary

	dd if=/dev/zero of=bin/os.bin bs=512 count=2880
	dd if=bin/bootloader.bin of=bin/os.bin conv=notrunc
	dd if=bin/kernel.bin of=bin/os.bin bs=512 seek=1 conv=notrunc
	qemu-system-x86_64 -drive format=raw,file=bin/os.bin

clean:
	rm -rf bin/* build/*