all:
	nasm -f bin -I src/ src/bootloader.asm -o bin/bootloader.bin
	dd if=/dev/zero of=bin/disk.img bs=512 count=2880
	dd if=bin/bootloader.bin of=bin/disk.img conv=notrunc
	qemu-system-x86_64 -drive format=raw,file=bin/disk.img

clean:
	rm -rf bin/*