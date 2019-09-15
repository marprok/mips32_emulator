#include "mips32.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE* in = NULL;
    if (argc != 2)
    {
        printf("./mips bin.mips\n");
        exit(1);
    }
    in = fopen(argv[1], "rb");
    if (!in)
    {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(1);
    }
    word inst = 0;
    word * w = (word*)inst_mem;
    while (fread(&inst, sizeof(inst), 1, in))
    {
        *w = inst;
        printf("0x%08X\n", inst);
        w++;
    }
    
    /*word * w = (word*)inst_mem;
     *w = 0x10050001;
     reg_file[5] = 0x0;
     reg_file[15] = 0xFF;
     w = (word*)(inst_mem + 12);
     *w = 0x1000FFFD;*/
    reg_file[1] = 1;
    reg_file[2] = 5;
    *((word*)(data_mem + 0)) = 0x123;
    emulate();

    fclose(in);
    return 0;
}
