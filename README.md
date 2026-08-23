# Shah Operating System

A small x86 (i686) operating system built from scratch in C and NASM assembly. It boots from a custom real-mode bootloader, transitions through protected mode, and brings up its own memory management, interrupt handling, and process scheduling — no existing kernel or bootloader is used.

## Features

- **Custom bootloader** (`src/boot/bootloader.asm`) — 16-bit real mode boot sector that enables the A20 line, loads the kernel from disk, retrieves a memory map via BIOS (`get_mmap.asm`), and switches the CPU into protected mode.
- **GDT / IDT setup** (`gdt.c`, `idt.c`) — Global and Interrupt Descriptor Tables configured for protected-mode segmentation and interrupt/exception handling, including a remapped PIC.
- **Interrupt Service Routines** (`isr.c`) — Handlers for hardware interrupts such as the keyboard.
- **Physical memory manager** (`pmm.c`) — Bitmap-based page frame allocator built from the BIOS memory map.
- **Paging / virtual memory** (`paging.c`, `vmm.c`) — Page directory/table setup, higher-half kernel mapping (`0xC0000000`), and page mapping/unmapping utilities.
- **Kernel heap** (`heap.c`) — Simple linked-list allocator (`kmalloc` / `kfree`) built on top of the paging layer.
- **Processes & ring 3** (`process.c`, `process_asm.asm`) — Basic process/context structures and the ability to create and run a user-mode (ring 3) process.
- **Drivers**
  - VGA text-mode screen driver (`screen.c`) with cursor control, scrolling, and colour support.
  - PS/2 keyboard driver (`keyboard.c`) with modifier key (ctrl/alt/shift) handling.
- **Helper library** (`string.c`) — Freestanding string utilities (no libc available in this environment).

## Project Structure

```
.
├── Makefile              # Build, run (QEMU), and debug (GDB) targets
├── linker.ld             # Kernel linker script (higher-half layout)
├── inc/                  # Public headers
│   ├── drivers/          # Screen, keyboard
│   ├── helper/           # String utilities
│   └── kernel/           # GDT, IDT, ISR, paging, PMM, VMM, heap, process
└── src/
    ├── boot/             # Real-mode bootloader + BIOS helpers (NASM)
    ├── drivers/          # Driver implementations
    ├── helper/           # String utility implementations
    └── kernel/           # Kernel entry point and subsystems
```

## Prerequisites

To build and run this OS you'll need:

- **NASM** — assembler for the bootloader and low-level kernel entry code
- **A cross-compiler** targeting `i686-elf` (the [OSDev cross-compiler guide](https://wiki.osdev.org/GCC_Cross-Compiler) covers how to build one), *or* a regular `gcc` with 32-bit support (`gcc-multilib`) as a fallback
- **QEMU** (`qemu-system-i386`) to run the OS in an emulator
- **GDB** (optional) — for kernel debugging

The Makefile automatically detects whether an `i686-elf` cross-compiler exists at `~/opt/cross`; if not, it falls back to the system `gcc` with `-m32`.

## Building & Running

Build the disk image and boot it in QEMU:

```bash
make
```

This will:
1. Assemble the bootloader (`bin/bootloader.bin`)
2. Compile and link the kernel (`bin/kernel.bin`)
3. Assemble a raw floppy-sized disk image (`bin/os.bin`) with the bootloader in the first sector and the kernel following it
4. Launch `qemu-system-i386` with that disk image

### Debugging with GDB

```bash
make gdb
```

Starts QEMU paused (`-s -S`) and attaches GDB to it, breaking at `main`.

### Other targets

```bash
make regs   # Run QEMU with interrupt/CPU-reset logging (useful for triple-fault debugging)
make clean  # Remove build artifacts
```

## Current Status

The kernel currently initializes the screen, GDT, IDT, physical memory manager, and paging, sets up a kernel heap, then creates and switches into a single user-mode (ring 3) process before enabling interrupts and idling. This is an active, in-progress hobby OS project — expect incomplete subsystems (e.g. no syscalls yet, no filesystem).

## License

No license file is currently included in this repository.
