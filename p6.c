#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2 ){
        printf("Usage: ./p6 <hexadecimal_address>\n");
        return -1;
    }

    unsigned int logical_address = strtoul(argv[1], NULL, 16);
    unsigned int offset = logical_address & 0xFFF;
    unsigned int page_index = logical_address >> 12;

    printf("Logical Addr:0x%08X - Page Index:0x%08X - Offset:0x%08X\n", logical_address, page_index, offset);
    return 0;
}