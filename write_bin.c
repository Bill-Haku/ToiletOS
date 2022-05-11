#include <stdio.h>

#define MBR_SIZE 512

int main(int argc, char *argv[]) {
    FILE *fp_disk = NULL;
    FILE *fp_mbr = NULL;

    fp_disk = fopen("disk.img", "wb");
    fp_mbr = fopen("mbr", "rb");

    char mbr[MBR_SIZE] = NULL;

}