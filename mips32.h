#ifndef MIPS_32
#define MIPS_32
#include <stdint.h>

typedef int8_t byte;
typedef int16_t half_word;
typedef int32_t word;

#define INST_MEM_SIZE 8*4 /* in bytes */

#define R 0x0
#define BEQ 0x4
#define SW 0x2B
#define LW 0x23

#define ALUOp0 0x1
#define ALUOp1 0x2
#define RegDest 0x4
#define RegWrite 0x8
#define ALUSrc 0x10
#define PCSrc 0x20
#define MemRead 0x40
#define MemWrite 0x80
#define MemToReg 0x100
#define Branch 0x200

word pc;
byte inst_mem[INST_MEM_SIZE];
word reg_file[32];
byte opcode;
byte rs;
byte rt;
byte rd;
byte shamt;
byte funct;
half_word immediate;
half_word signals;

void control_unit();
void emulate(void);
byte ALU_control_unit();
void inst_fetch(void);
void inst_decode(word inst);
void inst_execute();
void inst_mem_access(word result);
void inst_write_back(word result);
void dump_regs(void);

#endif // MIPS_32
