;/boot/MBR.asm
%include "boot.inc"     ;加载配置文件
SECTION MBR vstart=MBR_BASE_ADDR
;寄存器初始化   
    jmp start
    mbr_boot_print_str db 'MBR is running!'
    mbr_boot_print_str_len equ $-mbr_boot_print_str
    start:
    mov ax,cs           ;cs在boot后是重置为0的  以下部分将data code stack使用同一个段
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov fs,ax
    mov sp,0x7c00
    mov ax,0xb800
    mov gs,ax
;使用BIOS中断完成 MBR启动屏幕输出部分（方便查看MBR加载结果）
    mov ah,0x06
    mov al,0x0
    int 0x10      ;清屏
    mov cx,mbr_boot_print_str_len
    mov si,0
    mov di,0
    mov ah,0xF0
    mov bx,mbr_boot_print_str
    print_mbr_boot_loop:
    mov al,byte [bx+si]
    mov [gs:di],ax
    inc si
    add di,2
    dec cx
    cmp cx,0
    jnz print_mbr_boot_loop 

    mov eax,LOADER_START_LBA
    mov ebx,LOADER_BASE_ADDR
    mov ecx,LOADER_LBA_COUNT
    call rd_disk_m_16
    jmp LOADER_BASE_ADDR
;-------------------------------------------------------------------------------
;功能:读取硬盘n个扇区
rd_disk_m_16:      
;-------------------------------------------------------------------------------
                       ; eax=LBA扇区号
                       ; ebx=将数据写入的内存地址
                       ; ecx=读入的扇区数
      mov esi,eax     ;备份eax
      mov di,cx       ;备份cx
;读写硬盘:
;第1步：设置要读取的扇区数
      mov dx,0x1f2
      mov al,cl
      out dx,al            ;读取的扇区数

      mov eax,esi      ;恢复ax

;第2步：将LBA地址存入0x1f3 ~ 0x1f6

      ;LBA地址7~0位写入端口0x1f3
      mov dx,0x1f3                       
      out dx,al                          

      ;LBA地址15~8位写入端口0x1f4
      mov cl,8
      shr eax,cl
      mov dx,0x1f4
      out dx,al

      ;LBA地址23~16位写入端口0x1f5
      shr eax,cl
      mov dx,0x1f5
      out dx,al

      shr eax,cl
      and al,0x0f      ;lba第24~27位
      or al,0xe0       ; 设置7～4位为1110,表示lba模式
      mov dx,0x1f6
      out dx,al

;第3步：向0x1f7端口写入读命令，0x20 
      mov dx,0x1f7
      mov al,0x20                        
      out dx,al

;第4步：检测硬盘状态
  .not_ready:
      ;同一端口，写时表示写入命令字，读时表示读入硬盘状态
      nop
      in al,dx
      and al,0x88      ;第4位为1表示硬盘控制器已准备好数据传输，第7位为1表示硬盘忙
      cmp al,0x08
      jnz .not_ready       ;若未准备好，继续等。

;第5步：从0x1f0端口读数据
      mov ax, di
      mov dx, 256
      mul dx
      mov cx, ax       ; di为要读取的扇区数，一个扇区有512字节，每次读入一个字，
               ; 共需di*512/2次，所以di*256
      mov dx, 0x1f0
  .go_on_read:
      in ax,dx
      mov [bx],ax
      add bx,2        
      loop .go_on_read
      ret
times 510-($-$$) db 0
db 0x55,0xaa