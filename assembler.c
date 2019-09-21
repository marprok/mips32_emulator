/**
 * @file assembler.c
 * @author Marios Prokopakis
 * @date 21 Sep 2019
 * @brief A simple program that translates mips32 instructions to binary.
 *
 * THIS PROGRAM IS NOT A REAL ASSEMBLER.
 * It was created as a tool from the author in order for him
 * to avoid the painful(but educational) process of manualy
 * translating operations from text to binary.
 * The input file must contain instructions of the following
 * type : inst_name, reg1, reg2, reg3 for R type instructions
          inst_name, reg1, offset, base_address for LOAD/STORE instructions
          inst_name, reg1, reg2, offset for beq instruction
 * Registers must be in the form of $index
 */
#define _POSIX_C_SOURCE 200809L /*ssize_t and getline */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

/**
 * Remove all leading and trailing whitespaces from the
 * input string.
 * @param str Input/Output string
 */
static void trim(char** str)
{
    int i = strlen(*str) - 1;
    while (i >= 0 && isspace((*str)[i]))
    {
        (*str)[i] = '\0';
        i--;
    }
    while (isspace(**str))
        (*str)++;
}
/**
 * This function splits str into a bucket of tokens
 * according to a delimeter. If tokens is NULL, then
 * the function allocates memory to it. If it is not
 * NULL, then the contents are fread and the correct
 * amount of memory is allocated.
 * DO NOT PASS A POINTER TO A STATICLY ALLOCATED BUFFER
 * @param str The string to be split
 * @param delim The delimeter character
 * @param tokens The output bucket of tokens
 * @param zero Output variable that is set if the result is zero
 */
static int tokenize(char *str, char delim, char*** tokens)
{
    int count = 1;
    int i = 0;
    char *pointer = str;
    char *token = NULL;
    /* Count the tokens. */
    while (*pointer)
    {
        if (*pointer == delim)
            count++;
        pointer++;
    }
    
    if (tokens)
    {
        if (*tokens)
            free(*tokens);
        *tokens = (char**)malloc(count*sizeof(char*));
    }else
    {
        return -1;
    }

    pointer = str;
    token = str;

    while (count != 1 && *pointer)
    {
        if (*pointer == delim)
        {
            *pointer = '\0';
            trim(&token);
            (*tokens)[i] = token;
            token = pointer + 1;
            i++;
        }
        pointer++;
    }

    trim(&token);
    (*tokens)[i] = token;
    
    return count;
}
/**
 * This function returns the index of a register.
 * All registers must be in the form of $index
 * @param reg The first input register
 */
static uint8_t get_reg(const char* reg)
{
    return atoi(reg+1);
}
/**
 * This function performs a check upon a range
 * of registers to ensure that they are in the
 * form of $index.
 * @param regs An array of register names
 * @param The number of registers to check
 */
static char check_regs(char** regs, int count)
{
    int i = 0;
    for (; i < count; ++i)
    {
        if (strncmp(regs[i], "$", 1))
        {
            fprintf(stderr, "Invalid Instruction(%s)!\n", regs[i]);
            return 0;
        }
    }
    return 1;
}
/**
 * This function returns the immediate of an
 * instruction, which is a signed 16bit number.
 * @param reg The first input register
 */
static uint16_t get_immediate(char* immediate)
{
    char *c = immediate;
    if (*c == '-' || *c == '+')
        c++;
    while (*c)
    {
        if (*c < '0' || *c > '9')
        {
            fprintf(stderr, "Invalid immediate value(%s)!\n", immediate);
            exit(1);
        }
        c++;
    }
    return atoi(immediate);
}
/**
 * This function creates the binary representation of
 * an R type instruction.
 * @param opcode The operation code
 * @param s The rs register
 * @param t The rt register
 * @param d The rd register
 * @param shamt The shifting amount
 * @param funct The function code
 */
static uint32_t create_R(int8_t opcode, int8_t s, int8_t t,                        
                        int8_t d, int8_t shamt, int8_t funct)
{
    uint32_t inst = 0;
    inst = opcode;

    inst = inst << 5;
    inst |= s;

    inst = inst << 5;
    inst |= t;

    inst = inst << 5;
    inst |= d;

    inst = inst << 5;
    inst |= shamt;

    inst = inst << 6;
    inst |= funct;

    return inst;
}
/**
 * This function creates the binary representation of
 * an Immediate type instruction. The immediate must
 * be an unsinged 16bit int in order to prevent the sign
 * extention when performing operations such as or(|).
 * @param opcode The operation code
 * @param s The rs register
 * @param t The rt register
 * @param immediate The immediate amount
 */
static uint32_t create_Immediate(uint8_t opcode, uint8_t s, uint8_t t, uint16_t immediate)
{
    uint32_t inst = 0;
    inst = opcode;

    inst = inst << 5;
    inst |= s;

    inst = inst << 5;
    inst |= t;

    inst = inst << 16;
    inst |= immediate;

    return inst;
}
/**
 * This function encodes the text version of operations
 * to their corresponding binary form.
 * @param inst The buffer that contains the instruction components
 * @param count The number of components
 */
static uint32_t encode(char** inst, int count)
{
    uint32_t dinst = 0;
    if (count != 4)
    {
        fprintf(stderr, "Invalid Instruction(%d)!\n",count);
        exit(1);
    }
    if (!strcmp(inst[0], "add") || !strcmp(inst[0], "ADD"))
    {
        if (!check_regs(&inst[1], count-1))
            exit(1);
        dinst = create_R(0x0, get_reg(inst[2]), get_reg(inst[3]), get_reg(inst[1]), 0x0, 0x20);
    }else if (!strcmp(inst[0], "sub") || !strcmp(inst[0], "SUB"))
    {
        if (!check_regs(&inst[1], count-1))
            exit(1);
        dinst = create_R(0x0, get_reg(inst[2]), get_reg(inst[3]), get_reg(inst[1]), 0x0, 0x22);
    }else if (!strcmp(inst[0], "and") || !strcmp(inst[0], "AND"))
    {
        if (!check_regs(&inst[1], count-1))
            exit(1);
        dinst = create_R(0x0, get_reg(inst[2]), get_reg(inst[3]), get_reg(inst[1]), 0x0, 0x24);
    }else if (!strcmp(inst[0], "or") || !strcmp(inst[0], "OR"))
    {
        if (!check_regs(&inst[1], count-1))
            exit(1);
        dinst = create_R(0x0, get_reg(inst[2]), get_reg(inst[3]), get_reg(inst[1]), 0x0, 0x25);
    }else if (!strcmp(inst[0], "nor") || !strcmp(inst[0], "NOR"))
    {
        if (!check_regs(&inst[1], count-1))
            exit(1);
        dinst = create_R(0x0, get_reg(inst[2]), get_reg(inst[3]), get_reg(inst[1]), 0x0, 0x27);
    }else if (!strcmp(inst[0], "slt") || !strcmp(inst[0], "SLT"))
    {
        if (!check_regs(&inst[1], count-1))
            exit(1);
        dinst = create_R(0x0, get_reg(inst[2]), get_reg(inst[3]), get_reg(inst[1]), 0x0, 0x2A);
    }else if (!strcmp(inst[0], "beq") || !strcmp(inst[0], "BEQ"))
    {
        if (!check_regs(&inst[1], count-2))
            exit(1);
        dinst = create_Immediate(0x4, get_reg(inst[1]), get_reg(inst[2]), get_immediate(inst[3]));
    }else if (!strcmp(inst[0], "lw") || !strcmp(inst[0], "LW"))
    {
        if (!check_regs(&inst[1], 1) || !check_regs(&inst[3], 1))
            exit(1);
        dinst = create_Immediate(0x23, get_reg(inst[3]), get_reg(inst[1]), get_immediate(inst[2]));
    }else if (!strcmp(inst[0], "sw") || !strcmp(inst[0], "SW"))
    {
        if (!check_regs(&inst[1], 1) || !check_regs(&inst[3], 1))
            exit(1);
        dinst = create_Immediate(0x2B, get_reg(inst[3]), get_reg(inst[1]), get_immediate(inst[2]));
    }
    return dinst;
}

int main(int argc, char **argv)
{
    FILE* out = NULL;
    FILE* in = NULL;
    char ** tokens = NULL;
    char *inst = NULL;
    size_t len;
    ssize_t nread;
    int count;
    int32_t result;
    const char* bin = "bin.mips";

    if (argc != 2)
    {
        printf("./assembler file.mips\n");
        exit(1);
    }    

    out = fopen("bin.mips", "wb");
    if (!out)
    {
        fprintf(stderr, "Cannot create/open file %s\n", bin);
        exit(1);
    }

    in = fopen(argv[1], "r");
    if (!in)
    {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(1);
    }
    
    while ((nread = getline(&inst, &len, in)) > 0)
    {
        printf("line: %s", inst);
        count = tokenize(inst, ',', &tokens);
        if (count < 0)
        {
            fprintf(stderr, "Error while tokenizing the command %s\n", inst);
            exit(1);
        }
        result = encode(tokens, count);
        printf("0x%08X\n", result);
        fwrite(&result, sizeof(result), 1, out);
    }
    /* Cleanup */
    if (tokens)
        free(tokens);
    if (inst)
        free(inst);
    fclose(out);
    fclose(in);
    return 0;
}
