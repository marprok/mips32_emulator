/**
 * @file mips32.c
 * @author Marios Prokopakis
 * @date 21 Sep 2019
 * @brief A header file with all necessary declarations for the emulator.
 *
 * The implementation of the mips32.h header file.
 * Read the mips32.h description for more information.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mips32.h"

int32_t ALU(int32_t reg1, int32_t reg2, byte operation, byte *zero)
{
#ifdef DEBUG
    printf("ALU\n");
#endif
    int32_t result = 0;
    switch (operation)
    {
    case 0x2:
        result = reg1 + reg2;
        break;
    case 0x6:
        result = reg1 - reg2;
        break;
    case 0x0:
        result = reg1 & reg2;
        break;
    case 0x1:
        result = reg1 | reg2;
        break;
    case 0x7:
        result = reg1 < reg2 ? 1 : 0;
        break;
    case 0xC:
        result = ~(reg1 | reg2);
        break;
    }
    if (zero)
        *zero = result == 0;
    return result;
}

void control_unit(void)
{
#ifdef DEBUG
    printf("control_unit\n");
#endif
    signals = 0x0;
    printf("opcode: 0x%0X\n", opcode);
    switch (opcode)
    {
    case R:
        signals |= RegDest;
        signals |= RegWrite;
        signals |= ALUOp1;
        break;
    case BEQ:
        signals |= ALUOp0;
        signals |= Branch;
        break;
    case LW:
        signals |= ALUSrc;
        signals |= MemToReg;
        signals |= RegWrite;
        signals |= MemRead;
        break;
    case SW:
        signals |= ALUSrc;
        signals |= MemWrite;
        break;
    }
}

byte ALU_control_unit(void)
{
#ifdef DEBUG
    printf("ALU_control_unit\n");
#endif
    byte ALUOp = signals & 0x3;
    byte out_signal = 0;
    if (!ALUOp)
    {
        out_signal = 0x2;
    }else if (ALUOp & 0x01)
    {
        out_signal = 0x6;
    }else if (ALUOp & 0x02)
    {
        switch (funct)
        {
            /* ADD */
        case 0x20:
            out_signal = 0x2;
            break;
            /* SUB */
        case 0x22:
            out_signal = 0x6;
            break;
            /* AND */
        case 0x24:
            out_signal = 0x0;
            break;
            /* OR */
        case 0x25:
            out_signal = 0x1;
            break;
            /* NOR */
        case 0x27:
            out_signal = 0xC;
            break;
            /* SLT */
        case 0x2A:
            out_signal = 0x7;
            break;
        default:
            printf("Invalid ALU operation code: %d\n", funct);
            exit(1);
        }
    }
    return out_signal;
}

void emulate(void)
{
#ifdef DEBUG
    printf("emulate\n");
#endif
    dump_mem(reg_file, REG_SIZE/WORD_SIZE, "Initial State(REGS)");
    printf("\n");
    dump_mem((word*)data_mem, DATA_MEM_SIZE/WORD_SIZE, "Initial State(DATA)");
    printf("\n\n----Exucution----\n\n");

    while (pc < INST_MEM_SIZE)
    {
        printf("pc = %d\n", pc);
        inst_fetch();
        dump_mem(reg_file, REG_SIZE/WORD_SIZE, "Register State");
        dump_mem((word*)data_mem, DATA_MEM_SIZE/WORD_SIZE, "Memory State");
        sleep(1);
    }
}

void dump_mem(word* buffer, uint32_t buffer_size, const char* msg)
{
#ifdef DEBUG
    printf("dump_mem\n");
#endif
    uint32_t i;
    printf("----%s----\n", msg);
    for (i = 0; i < buffer_size; ++i)
    {
        if (i % 4 == 0)
            printf("%d-%d:\t", i,i+3);
        printf("0x%08X ",buffer[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
}

void inst_fetch(void)
{
#ifdef DEBUG
    printf("inst_fetch\n");
#endif
    word inst = *((word*)(inst_mem + pc));
    pc += WORD_SIZE;
    inst_decode(inst);
}

void inst_decode(word inst)
{
#ifdef DEBUG
    printf("inst_decode\n");
#endif
    opcode = inst >> 26;
    rs = (inst >> 21) & 0x1F;
    rt = (inst >> 16) & 0x1F;
    rd = (inst >> 11) & 0x1F;
    shamt = (inst >> 6) & 0x1F;
    funct = inst & 0x3F;
    immediate = inst & 0xFFFF;
    printf("rs = %d\n"
           "rt = %d\n"
           "rd = %d\n"
           "shampt = %d\n"
           "funct = %d\n"
           "immediate = %d\n",
           rs, rt, rd, shamt, funct, immediate);

    control_unit();
    inst_execute();
}

void inst_execute(void)
{
#ifdef DEBUG
    printf("inst_execute\n");
#endif
    word result = 0;
    int32_t reg1 = reg_file[rs];
    int32_t reg2;
    byte zero;

    if (signals & ALUSrc)
        reg2 = immediate;
    else
        reg2 = reg_file[rt];

    result = ALU(reg1, reg2, ALU_control_unit(), &zero);

    if (zero && Branch & signals)
        signals |= PCSrc;

    inst_mem_access(result);
}

void inst_mem_access(word result)
{
#ifdef DEBUG
    printf("inst_mem_access\n");
#endif
    word mem_data = 0;
    if (signals & MemRead)
    {
        /* Load Word */
        mem_data = *((word*)(data_mem + result));
        printf("loaded: %d\n", mem_data);
    }
    if (signals & MemWrite)
    {
        /* Store Word */
        *((word*)(data_mem + result)) = reg_file[rt];
    }
    if (signals & MemToReg)
        result = mem_data;
    inst_write_back(result);
}

void inst_write_back(word result)
{
#ifdef DEBUG
    printf("inst_write_back\n");
#endif
    if (signals & PCSrc)
        pc += immediate << 2;

    if (signals & RegWrite)
    {
        if (signals & RegDest)
            reg_file[rd] = result;
        else
            reg_file[rt] = result;
    }
}
