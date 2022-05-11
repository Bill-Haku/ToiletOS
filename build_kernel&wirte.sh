# build kernel by nasm
nasm kernel.asm -o kernel
# write into disk
dd if=kernel of=disk.img bs=512 seek=100 conv=notrunc