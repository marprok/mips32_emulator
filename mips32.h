/**
 * @file mips32.h
 * @author Marios Prokopakis
 * @date 21 Sep 2019
 * @brief A header file with all necessary declarations for the emulator.
 *
 * This Program emulates a non-pipelined, single cycled MIPS32 processor.
 * The current implementation supports only a subset of the instructions
 * that a real MIPS processor supprts.
 * The supported instructions are the following: lw, sw, add, sub, and,
 *                                               or, nor, slt, beq
 * This implementation is heavily based on the book Computer Organization
 * and Design 4th Edition, by David A. Patterson and John L. Hennessy and
 * it was created by the author as a way to get a better understanding
 * of the CPU internals of a non-pipelined RISK processor.
 */

#ifndef MIPS_32
#define MIPS_32
#include <stdint.h>

#define DEBUG_no
/**
 * Type declarations.
 */
typedef uint8_t       byte;
typedef uint16_t half_word;
typedef uint32_t      word;

/**
 * Type sizes in bytes.
 */
#define      WORD_SIZE 4
#define HALF_WORD_SIZE 2
#define      BYTE_SIZE 1
/**
 * Buffer sizes in bytes.
 */
#define INST_MEM_SIZE  8*WORD_SIZE /* in bytes */
#define DATA_MEM_SIZE  8*WORD_SIZE /* in bytes */
#define      REG_SIZE 32*WORD_SIZE /* in bytes */
/**
 * Operation codes.
 */
#define   R 0x0
#define BEQ 0x4
#define  SW 0x2B
#define  LW 0x23
/**
 * Signal masks.
 */
#define   ALUOp0   0x1
#define   ALUOp1   0x2
#define  RegDest   0x4
#define RegWrite   0x8
#define   ALUSrc  0x10
#define    PCSrc  0x20
#define  MemRead  0x40
#define MemWrite  0x80
#define MemToReg 0x100
#define   Branch 0x200
/**
 * Instruction program counter, memory buffers, register file,
 * program counter and signals
 */
word pc;
byte inst_mem[INST_MEM_SIZE];
byte data_mem[DATA_MEM_SIZE];
word reg_file[REG_SIZE/WORD_SIZE];
byte opcode;
byte rs;
byte rt;
byte rd;
byte shamt;
byte funct;
int16_t immediate; /* signed 16 bit number */
half_word signals;

/**
 * This function performs the ALU operations
 * @param reg1 The first input register
 * @param reg2 The second input register
 * @param operation The ALU operation to be executed
 * @param zero Output variable that is set if the result is zero
 */
int32_t ALU(int32_t reg1, int32_t reg2, byte operation,
                                            byte *zero);
/**
 * This function inititates the emulation process.
 */
void emulate(void);
/**
 * The control unit sets the necessary signals according
 * to the operation code of the current instruction.
 */
void control_unit(void);
/**
 * The ALU control unit, decides which ALU operation
 * must be performed for each instruction.
 */
byte ALU_control_unit(void);
/**
 * This stage fetches the next instruction from the
 * instruction memory and increments the program counter
 * so that it points to the next instruction.
 */
void inst_fetch(void);
/**
 * This stage decodes the current instruction and
 * initializes the instruction components.
 */
void inst_decode(word inst);
/**
 * This stage executes the current instruction.
 */
void inst_execute(void);
/**
 * This stage performs the data memory access for the
 * sw and lw instructions.
 */
void inst_mem_access(word result);
/**
 * This stage writes data to the register file
 * and updates the program counter in case of a
 * beq instruction.
 */
void inst_write_back(word result);
/**
 * This is a helper funtion that is used to print
 * the contents of a memory buffer to the screen.
 */
void dump_mem(word* buffer, uint32_t buffer_size,
                                const char* msg);

#endif // MIPS_32
