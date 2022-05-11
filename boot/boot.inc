;/boot/boot.inc
MBR_BASE_ADDR equ 0x7c00        ;MBR加载地址
LOADER_BASE_ADDR equ 0x900      ;loader加载位置
LOADER_STACK_TOP equ 0x900      ;注意stack、code、data是使用同一个段的
LOADER_START_LBA equ 1          ;LOADER起始逻辑扇区号
LOADER_LBA_COUNT equ 4          ;LOADER所占用扇区