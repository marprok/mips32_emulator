# MIPS 32 Emulator

This repository contains an implementation of a **non-pipelined MIPS 32** processor.
The emulator supports only a **subset** of the MIPS 32 instruction set and it
is based on the book **Computer Organization and Design 4th Edition,
by David A. Patterson and John L. Hennessy(Chapter 4)**. The purpose of this project
was to get a better understanding of the internals of a non-pipelinged single
cycled RISK processor.

### Supported Instructions

|Instruction |
|------------|
| lw         |
| sw         |
| add        |
| sub        |
| and        |
| or         |
| nor        |
| slt        |
| beq        |

### The assembler program

**This is not a real assembler program**

It was created as a tool from the author in order for him to avoid the
painful(but educational) process of manualy translating operations from
text to binary.
The assembly program must contain a sigle instruction per line and
all the instructions must be of the following type.

* inst_name, reg1, reg2, reg3 (for R type instructions)
* inst_name, reg1, offset, base_address (for LOAD/STORE instructions)
* inst_name, reg1, reg2, offset (for beq instruction)
* Registers must be in the form of $index

The file **test.mips** contains an example program.

### Prerequisites

**GCC**, **make** and **git** if you want to clone the project from GitHub.

### Installing

1. git clone https://github.com/marprok/mips32_emulator
2. cd mips32_emulator
3. make
4. make assembler

### Running the example program

1. ./assembler test.mips
2. ./main bin.mips


