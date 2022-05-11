#!Makefile

all: MBR LOADER

.PHONY:MBR
MBR:
	nasm -f bin MBR.s -o mbr.bin
	dd if=MBR.bin of=../disk.img seek=0
.PHONY:LOADER
LOADER:
	nasm -f bin LOADER.s -o loader.bin
	dd if=LOADER.bin of=../disk.img seek=1
.PHONY:run
run:
	qemu ../disk.img
.PHONY:qemu
qemu:
	qemu ../disk.img