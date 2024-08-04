# Binary Translator - CSCI 2122 Assignment 4

## Overview

This assignment involves creating a binary translator in C that translates a simplified 16-bit instruction set (X) into x86-64 assembly code. The translator will read the binary file containing X instructions, translate these into x86-64 instructions, and output the corresponding assembly code. The project is divided into two parts, with the second part extending the functionality of the translator to handle more complex instructions.

## Objectives

- Develop a binary translator to convert a simplified instruction set into x86-64 assembly.


## Project Structure

- `xtra/`: Directory where code is to be written.
- `xtra/tests/`: Directory containing tests that will be executed each time code is submitted.
- `Makefile`: Sample Makefile for building the program.
- `.gitlab-ci.yml`: CI configuration file (do not modify).
- `main.c`: Source file containing the `main()` function and the implementation of the translator.

## Instruction Set

The X instruction set is a simplified RISC-based 16-bit instruction set with 30 instructions categorized into:

1. 0-operand instructions
2. 1-operand instructions
3. 2-operand instructions
4. Extended instructions

Each instruction is encoded in a specific format. Refer to the assignment document for detailed encoding schemes and examples.

## Task 1: Implement the Simple Xtra

1. **Command-line Argument**: The translator should take one argument - the binary file to be translated.
   ```sh
   ./xtra <filename>
   ```

2. **File Handling**: Open the specified file for reading.
3. **Prologue**: Output a simple prologue.
4. **Translation Loop**:
   - Read an instruction.
   - If the instruction is `0x00 0x00`, break out of the loop.
   - Translate the instruction and output the corresponding x86-64 assembly.
5. **Epilogue**: Output a simple epilogue.

### Prologue and Epilogue

- **Prologue**:
  ```asm
  .globl _start
  _start:
      push %rbp
      mov %rsp, %rbp
  ```

- **Epilogue**:
  ```asm
      pop %rbp
      ret
  ```

## Register Mapping

The X architecture has 16 general-purpose registers and a status register (F), mapped to x86-64 registers as follows:

| X Register | x86-64 Register |
|------------|------------------|
| r0         | %rax             |
| r1         | %rbx             |
| r2         | %rcx             |
| r3         | %rdx             |
| r4         | %rsi             |
| r5         | %rdi             |
| r6         | %r8              |
| r7         | %r9              |
| r8         | %r10             |
| r9         | %r11             |
| r10        | %r12             |
| r11        | %r13             |
| r12        | %r14             |
| F          | %r15             |
| r14        | %rbp             |
| r15        | %rsp             |

## Debug Mode

- **std (Start Debug Mode)**: Set an internal debug flag in the translator.
- **cld (Stop Debug Mode)**: Clear the internal debug flag.

When the debug flag is true, output the debug assembly code before translating each X instruction.

## Output Handling

For the `out rN` instruction, translate it to a call to the `outchar(char c)` function. Save the current value of `%rdi`, move the character to be printed into `%rdi`, call `outchar`, and restore `%rdi`.

## Compilation

To compile the program, run:
```sh
make
```

## Usage

To run the translator, use:
```sh
./xtra <filename>
```

