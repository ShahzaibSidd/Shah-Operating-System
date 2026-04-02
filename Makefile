all:
	nasm -f bin -I src/ src/bootloader.asm -o bin/bootloader.bin
	qemu-system-x86_64 bin/bootloader.bin

clean:
	rm -rf bin/*