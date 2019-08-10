#include <stdio.h>
#include "mips32.h"


void emulate()
{
	while (pc < INST_MEM_SIZE)
	{
		inst_fetch();
		dump_regs();
		pc += 4;
	}
}

void inst_fetch()
{
	word inst = *((word*)(inst_mem + pc));
	inst_decode(inst);
}

void inst_decode(word inst)
{
	opcode = inst >> 26;
	switch (opcode)
	{
	case R:
		decode_R();
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
		/*TODO substract for branch */
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

void decode_R(word inst)
{
	rs = (inst >> 21) & 0x1F;
	rt = (inst >> 16) & 0x1F;
	rd = (inst >> 11) & 0x1F;
	shamt = (inst >> 6) & 0x1F;
	funct = inst & 0x3F;
	
	printf("rs = %d\n"
		   "rt = %d\n"
		   "rd = %d\n"
		   "shampt = %d\n"
		   "funct = %d\n",
		   rs, rt, rd, shamt, funct);
	inst_execute(0x02);	
}
void dump_regs()
{
	uint32_t i;
	for (i = 0; i < 32; ++i)
		printf("reg_file[%d] = %d\n",i, reg_file[i]);
}


