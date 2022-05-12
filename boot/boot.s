;/boot/boot.s

%include "boot.inc" 
;这三个双字为GRUB加载器识别MagicNumber 以及配置信息（可以不用理解）
dd 0x1badb002  
dd 0x3   	
dd -(0x1badb002+0x3)      

[BITS 32]   ;由于GRUB在加载内核前进入保护模式，所以要32位编译   
section .text
[GLOBAL boot_start]    
[GLOBAL mutiboot_addr32]  
[EXTERN kern_entry]
   GDT_BASE:   dd    0x00000000 
           	   dd    0x00000000

   CODE_DESC:  dd    0x0000FFFF 
               dd    DESC_CODE_HIGH4

   DATA_STACK_DESC:  dd    0x0000FFFF
                     dd    DESC_DATA_HIGH4

   VIDEO_DESC: dd    0x80000007        ; limit=(0xbffff-0xb8000)/4k=0x7
               dd    DESC_VIDEO_HIGH4  ; 此时dpl为0

   GDT_SIZE   equ   $ - GDT_BASE
   GDT_LIMIT   equ   GDT_SIZE - 1 
   times 30 dq 0                     ; 此处预留30个描述符的空位
   SELECTOR_CODE equ (0x0001<<3) + TI_GDT + RPL0     ; 相当于(CODE_DESC - GDT_BASE)/8 + TI_GDT + RPL0
   SELECTOR_DATA equ (0x0002<<3) + TI_GDT + RPL0     ; 同上
   SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0    ; 同上 

   total_mem_bytes dd 0                  
   ;以下是定义gdt的指针，前2字节是gdt界限，后4字节是gdt起始地址
   gdt_ptr  dw  GDT_LIMIT 
        	dd  GDT_BASE
;boot开始！
boot_start:
    cli                        ;关闭外中断        
    mov [mutiboot_addr32], ebx ; GRUB加载内核后会将mutiboot信息地址存放在ebx中
    ;-----------------   准备进入保护模式   -------------------
;1 打开A20
;2 加载gdt
;3 将cr0的pe位置1
   ;-----------------  打开A20  ----------------
    in al,0x92
    or al,0000_0010B
    out 0x92,al
   ;-----------------  加载GDT  ----------------
    lgdt [gdt_ptr]
   ;-----------------  cr0第0位置1  ----------------
    mov eax, cr0
    or eax, 0x00000001
    mov cr0, eax
    jmp dword SELECTOR_CODE:far_jmp_target      ; 刷新流水线，避免分支预测的影响,这种cpu优化策略，最怕jmp跳转，

;初始化段寄存器以及栈结构
    far_jmp_target:
    mov ax,SELECTOR_DATA
    mov ss,ax
    mov ds,ax
  	mov ax,SELECTOR_VIDEO
  	mov gs,ax
    mov esp, STACK_TOP      
    and esp, 0xFFFFFFF0  ;16字节对齐
    mov ebp, 0           
;进入内核主函数    
    call kern_entry                    
    jmp dword $          ;防止意外退出内核

section .data
mutiboot_addr32:        
    dd 0x0        

section .bss             ; 未初始化的数据段从这里开始
stack:
    resb 0x100000        ; 1M的内核栈 (应该够了吧,不够自己改)
STACK_TOP equ $-1 