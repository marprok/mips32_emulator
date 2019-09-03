#ifndef MIPS_32
#define MIPS_32
#include <stdint.h>

typedef int8_t byte;
typedef int16_t half_word;
typedef int32_t word;

#define INST_MEM_SIZE 4*4 /* in bytes */
#define R 0x0
#define BEQ 0x4

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

void emulate(void);
byte ALU_control_unit(byte funct, byte ALUOp);
void inst_fetch(void);
void inst_decode(word inst);
void inst_execute(byte ALUop);
void inst_mem_acces(void);
void inst_write_back(word result);
void dump_regs(void);

#endif // MIPS_32
