#include <stdio.h>
#include "mips32.h"
#include <unistd.h>
#include <stdlib.h>

/*
  A single cycled implementation of a MIPS CPU.
  All the implementation is based on the single cycled
  architecture that is presented in chapter 4
  of the Patterson, Hennessy book of Computer Architecture.
*/

byte ALU_control_unit(byte funct, byte ALUOp)
{
    byte out_signal = 0;
    if (!ALUOp)
    {
	/* TODO memory access */
	out_signal = 0x2;
    }else if (ALUOp & 0x01)
    {
	out_signal = 0x6;
    }else if (ALUOp & 0x02)
    {
	switch (funct)
	{
	case 0x20:
	    out_signal = 0x2;
	    break;
	case 0x22:
	    out_signal = 0x6;
	    break;
	case 0x24:
	    out_signal = 0x0;
	    break;
	case 0x25:
	    out_signal = 0x1;
	    break;
	case 0x27:
	    out_signal = 0xC;
	    break;
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

/* The function that starts the emulation. */
void emulate(void)
{
    printf("Initial register state:\n");
    dump_regs();
    printf("\n\n");
    while (pc < INST_MEM_SIZE)
    {
	printf("pc = %d\n", pc);
	inst_fetch();
	dump_regs();
	sleep(1);
    }
}

/* Print the contents of the register file.*/
void dump_regs(void)
{
    uint32_t i;
    for (i = 0; i < 32; ++i)
    {
	if (i % 4 == 0)
	    printf("%d-%d:\t", i,i+3);
	printf("0x%08X ",reg_file[i]);
	if ((i + 1) % 4 == 0)
	    printf("\n");
    }
}

/* Each of the following funtions is mapped to 
   one of the stages of a clock cycle.
   These stages are the fillowing:
   INSTRUCTION FETCH
   INSTRUCTION DECODE
   INSTRUCTION EXECUTION
   INSTRUCTION MEMORY ACCESS
   REGISTER WRITE BACK
*/

/* The instruction fetch stage. */
void inst_fetch(void)
{
    word inst = *((word*)(inst_mem + pc));
    pc += 4;
    inst_decode(inst);
}

/* The instruction decode stage. */
void inst_decode(word inst)
{
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
    switch (opcode)
    {
    case R:
	inst_execute(0x02);
	break;
    case BEQ:
	inst_execute(0x01);
	break;
    default:
	printf("Not Supported opcode!\n");
    }
}

/*
   The instruction execution stage.
   ALUOp: Patterson, Hennessy chapter 4, page 375
*/
void inst_execute(byte ALUOp)
{
    word result = 0;
    word reg1 = reg_file[rs];
    word reg2 = reg_file[rt];

    switch (ALU_control_unit(funct, ALUOp))
    {
    case 0x2: // add
	result = reg1 + reg2;
	break;
    case 0x6: // sub
	result = reg1 - reg2;
	break;
    case 0x0: // and
	result = reg1 & reg2;
	break;
    case 0x1: // or
	result = reg1 | reg2;
	break;
    case 0x7: // slt
	result = reg1 < reg2 ? 1 : 0;
	break;
    }

    switch (ALUOp)
    {
    case 0x0:
	/* TODO memory access */
	break;
    case 0x1:
	/* If it is a branch instruction. */
	if (!result)
	{
	    pc += immediate << 2;
	}
	break;
    case 0x2:
	inst_write_back(result);
	break;
    }
}

/* The memory access stage. */
void inst_mem_access(void)
{
	
}

/* The register write back stage. */
void inst_write_back(word result)
{
    reg_file[rd] = result;
}
