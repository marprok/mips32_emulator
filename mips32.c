#include <stdio.h>
#include "mips32.h"
#include <unistd.h>

void emulate()
{
	while (pc < INST_MEM_SIZE)
	{
		printf("pc = %d\n", pc);
		inst_fetch();
		dump_regs();
		sleep(1);
	}
}

void inst_fetch()
{
	word inst = *((word*)(inst_mem + pc));
	pc += 4;
	inst_decode(inst);
}

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

void inst_execute(byte ALUop)
{
	word result = 0;
	word reg1 = reg_file[rs];
	word reg2 = reg_file[rt];
	if (!ALUop)
	{
		/* TODO memory access */
	}else if (ALUop & 0x01)
	{
		result = reg1 - reg2;
		if (!result)
		{
			pc += immediate << 2;
			printf("beq: %d\n", result);
		}
		printf("here\n");
	}else if (ALUop & 0x02)
	{
		switch (funct)
		{
		case 0x20:
			result = reg1 + reg2;
			break;
		case 0x22:
			result = reg1 - reg2;
			break;
		case 0x24:
			result = reg1 & reg2;
			break;
		case 0x25:
			result = reg1 | reg2;
			break;
		case 0x27:
			result = ~(reg1 | reg2);
			break;
		case 0x2A:
			result = reg1 < reg2 ? 1 : 0;
			break;
		default:
			printf("Invalid ALU operation code: %d\n", funct);
		}
		inst_write_back(result);
	}
}

void inst_mem_access()
{
	
}

void inst_write_back(word result)
{
	reg_file[rd] = result;
}

void dump_regs()
{
	uint32_t i;
	for (i = 0; i < 32; ++i)
		printf("reg_file[%d] = %d\n",i, reg_file[i]);
}


