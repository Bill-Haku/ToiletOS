#!Makefile

C_SOURCES = $(shell find . -name "*.c")        #.c源文件
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES)) #.c生成.o文件
S_SOURCES = $(shell find . -name "*.s")        #.s源文件
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES)) #.s生成.o文件


#编译器与链接器参数
nasm_pars = -f elf -g -F stabs
gcc_pars = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include 
ld_pars = -T kernel.ld -m elf_i386 -nostdlib

#目标软盘
target_floppy = floppy.img

#过程
all: $(S_OBJECTS) $(C_OBJECTS) link copykern

.c.o:
	gcc $(gcc_pars) $< -o $@

.s.o:
	nasm $(nasm_pars) $<

.PHONY:link
link:
	ld $(ld_pars) $(S_OBJECTS) $(C_OBJECTS) -o kernel.elf

.PHONY:copykern
copykern:
	sudo mount $(target_floppy) /mnt/kernel/
	sudo cp kernel.elf /mnt/kernel/
	sudo umount /mnt/kernel/
	
.PHONY:mt
mt:
	sudo mount $(target_floppy) /mnt/kernel

.PHONY:umt
umt:
	sudo umount /mnt/kernel

.PHONY:run
run:
	qemu -fda $(target_floppy) -boot a -m 256

.PHONY:clean
clean:
	rm $(S_OBJECTS) $(C_OBJECTS) kernel.elf

.PHONY:debug
debug:
	qemu -S -s -fda $(target_floppy) -boot a &
	sleep 1
	gdb -x gdb.script