BUILD_DIR = build
ENTRY_POINT = 0xc0001500
target_floppy = disk.img
AS = nasm
CC = gcc
LD = ld
LIB = -I include
ASFLAGS = -f elf
ASIB = -I include/
CFLAGS = -Wall -m32 -fno-stack-protector $(LIB) -c -fno-builtin -W -Wstrict-prototypes -Wmissing-prototypes
LDFLAGS = -m elf_i386 -Ttext $(ENTRY_POINT) -e main -Map $(BUILD_DIR)/kernel.map
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/entry.o $(BUILD_DIR)/printk.o $(BUILD_DIR)/string.o $(BUILD_DIR)/vga_basic.o $(BUILD_DIR)/port.o ${BUILD_DIR}/print.o
# OBJS = $(BUILD_DIR)/main.o ${BUILD_DIR}/print.o

# C代码编译
$(BUILD_DIR)/entry.o: boot/entry.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/port.o: libs/port.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/printk.o: libs/printk.c 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/string.o: libs/string.c 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/vga_basic.o: libs/vga_basic.c 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/main.o: kernel/main.c 
	$(CC) $(CFLAGS) $< -o $@

# 编译loader和mbr
$(BUILD_DIR)/mbr.bin: mbr/mbr.S
	$(AS) $(ASIB) $< -o $@

$(BUILD_DIR)/loader.bin: mbr/loader.S
	$(AS) $(ASIB) $< -o $@

# 编译汇编
# $(BUILD_DIR)/kernel.o: kernel/kernel.asm
# 	$(AS) $(ASFLAGS) $< -o $@

# $(BUILD_DIR)/print.o: lib/kernel/print.asm
# 	$(AS) $(ASFLAGS) $< -o $@

# 链接
$(BUILD_DIR)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: mk_dir hd clean all ab mkbochs

mk_dir:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	qemu-img create -f qcow2 -o preallocation=metadata disk.img 4M
	echo "Create image done."

hd:
	dd if=$(BUILD_DIR)/mbr.bin of=disk.img bs=512 count=1 conv=notrunc
	dd if=$(BUILD_DIR)/loader.bin of=disk.img bs=512 count=4 seek=2 conv=notrunc
	dd if=$(BUILD_DIR)/kernel.bin of=disk.img bs=512 count=200 seek=9 conv=notrunc

mkbochs:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	bximage -hd=10M -mode="create" -q disk.img

clean:
	rm -rf disk.img $(BUILD_DIR)

.PHONY:run
run:
	qemu -drive file=$(target_floppy),format=raw,media=disk -boot c -m 256 --no-reboot

.PHONY:rb
rb:
	bochs -f bochsrc

build: $(BUILD_DIR)/mbr.bin $(BUILD_DIR)/loader.bin $(BUILD_DIR)/kernel.bin

all: mk_dir build hd

ab: mkbochs build hd