%include "boot.inc"
section LOADER vstart=LOADER_BASE_ADDR
    mov cx,loader_boot_print_str_len
    mov si,0
    mov di,0
    mov ah,0xF0
    mov bx,loader_boot_print_str
    print_loader_boot_loop:
    mov al,byte [bx+si]
    mov [gs:di],ax
    inc si
    add di,2
    dec cx
    cmp cx,0
    jnz print_loader_boot_loop
    jmp $
    loader_boot_print_str dd "This is the ToiletOS LOADER!"
    loader_boot_print_str_len equ $-loader_boot_print_str