/**
 * @file main.c
 * @author Marios Prokopakis
 * @date 21 Sep 2019
 * @brief The main function of the emulator program.
 *
 * The data memory contents and the register files
 * are initialized in the main function.
 */
#include "mips32.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE* in = NULL;
    word inst = 0;
    word * w;

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

    inst = 0;
    w = (word*)inst_mem;
    while (fread(&inst, sizeof(inst), 1, in))
    {
        *w = inst;
        printf("0x%08X\n", inst);
        w++;
    }
    
    reg_file[1] = 1;
    reg_file[2] = 5;
    *((word*)(data_mem + 0)) = -123;

    emulate();

    fclose(in);
    return 0;
}
