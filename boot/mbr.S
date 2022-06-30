;/boot/MBR.asm
; 主引导程序
;-----------------------------------------------
%include "boot.inc"
SECTION MBR vstart=0x7c00

    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov sp, 0x7c00
    mov ax, 0xb800
    mov gs, ax

; 清屏
;---------------------------------------------------
    mov ax, 0600h
    mov bx, 0700h
    mov cx, 0
    mov dx, 184fh
    int 10h

    ; 显示"MBR"
    mov byte [gs:0x00], 'T'
    mov byte [gs:0x01], 0x24

    mov byte [gs:0x02], 'o'
    mov byte [gs:0x03], 0x24

    mov byte [gs:0x04], 'i'
    mov byte [gs:0x05], 0x24

    mov byte [gs:0x06], 'l'
    mov byte [gs:0x07], 0x24

    mov byte [gs:0x08], 'e'
    mov byte [gs:0x09], 0x24

    mov byte [gs:0x0A], 't'
    mov byte [gs:0x0B], 0x24

    mov byte [gs:0x0C], 'O'
    mov byte [gs:0x0D], 0x24

    mov byte [gs:0x0E], 'S'
    mov byte [gs:0x0F], 0x24

    mov byte [gs:0x10], ' '
    ;mov byte [gs:0x11], 0xA4

    mov byte [gs:0x12], '-'
    ;mov byte [gs:0x13], 0xA4

    mov byte [gs:0x14], ''
    ;mov byte [gs:0x15], 0xA4

    mov byte [gs:0x16], 'B'
    ;mov byte [gs:0x17], 0xA4

    mov byte [gs:0x18], 'i'
    ;mov byte [gs:0x19], 0xA4

    mov byte [gs:0x1A], 'l'
    ;mov byte [gs:0x1B], 0xA4

    mov byte [gs:0x1C], 'l'
    ;mov byte [gs:0x1D], 0xA4

    mov byte [gs:0x1E], ' '
    ;mov byte [gs:0x1F], 0xA4

    mov byte [gs:0x20], 'H'
    ;mov byte [gs:0x21], 0xA4

    mov byte [gs:0x22], 'a'
    ;mov byte [gs:0x23], 0xA4

    mov byte [gs:0x24], 'k'
    ;mov byte [gs:0x25], 0xA4

    mov byte [gs:0x26], 'u'
    ;mov byte [gs:0x27], 0xA4

    mov eax, LOADER_START_SECTOR
    mov bx, LOADER_BASE_ADDR
    
    ; 读取4个扇区
    mov cx, 4
    call rd_disk_m_16

    ; 直接跳到loader的起始代码执行
    jmp LOADER_BASE_ADDR + 0x300

;-----------------------------------------------------------
; 读取磁盘的n个扇区，用于加载loader
; eax保存从硬盘读取到的数据的保存地址，ebx为起始扇区，cx为读取的扇区数
rd_disk_m_16:
;-----------------------------------------------------------

    mov esi, eax
    mov di, cx

    mov dx, 0x1f2
    mov al, cl
    out dx, al

    mov eax, esi

    mov dx, 0x1f3
    out dx, al

    mov cl, 8
    shr eax, cl
    mov dx, 0x1f4
    out dx, al

    shr eax, cl
    mov dx, 0x1f5
    out dx, al

    shr eax, cl
    and al, 0x0f
    or al, 0xe0
    mov dx, 0x1f6
    out dx, al

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

.not_ready:
    nop
    in al, dx
    and al, 0x88
    cmp al, 0x08
    jnz .not_ready

    mov ax, di
    mov dx, 256
    mul dx
    mov cx, ax
    mov dx, 0x1f0

.go_on_read:
    in ax, dx
    mov [bx], ax
    add bx, 2
    loop .go_on_read
    ret

    times 510-($-$$) db 0
    db 0x55, 0xaa
