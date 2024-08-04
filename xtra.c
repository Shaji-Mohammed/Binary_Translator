//
// Created by SUFIYAN on 2024-03-24.
//

#include <stdio.h>
#include "xtra.h"
#include "xis.h"

unsigned short convertToShort(char* buffer) {
    return (short)((buffer[0] << 8) | buffer[1]);
}

void translate_instructions(FILE *file) {
    const char* registerMap[] = {
            "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", "r9",
            "r10", "r11", "r12", "r13", "r14", "r15", "rbp", "rsp"
    };

    short instruction;
    char buffer[100];
    short debug_flag = 0;
    unsigned short f_flag = 0;
    unsigned short label;

    while ((fread(buffer, 1, 2, file) == 2)){
        instruction = convertToShort(buffer);
        short reg1;
        short reg2;

        if (instruction == 0x0000) {
            break;
        }

        if (debug_flag == 1) {
            printf("\tcall debug\n");
        }

        switch ((instruction >> 14) & 0x03) {
            case 0:     // 0-operand instruction
                switch ((instruction >> 8) & 0x3F) {
                    case I_RET:         //ret
                        printf("\tret\n");
                        break;
                    case I_CLD:
                        debug_flag = 0;
                        break;
                    case I_STD:
                        debug_flag = 1;
                        break;
                }
                break;

            case 1:     // 1-operand instruction
                switch ((instruction >> 8) & 0xFF) {
                    case I_NEG:
                        reg1 = ((instruction >> 4) & 0xF);
                        printf("\tneg %%%s\n", registerMap[reg1]);
                        break;

                    case I_NOT:
                        reg1 = ((instruction >> 4) & 0xF);
                        printf("\tnot %%%s\n", registerMap[reg1]);
                        break;

                    case I_INC:
                        reg1 = ((instruction >> 4) & 0xF);
                        printf("\tinc %%%s\n", registerMap[reg1]);
                        break;

                    case I_DEC:
                        reg1 = ((instruction >> 4) & 0xF);
                        printf("\tdec %%%s\n", registerMap[reg1]);
                        break;

                    case I_PUSH:
                        reg1 = ((instruction >> 4) & 0xF);
                        printf("\tpush %%%s\n", registerMap[reg1]);
                        break;

                    case I_POP:
                        reg1 = ((instruction >> 4) & 0xF);
                        printf("\tpop %%%s\n", registerMap[reg1]);
                        break;

                    case I_OUT:
                        reg1 = ((instruction >> 4) & 0xF);
                        printf("\tpush %%%s\n", registerMap[5]);
                        printf("\tmovq %%%s, %%rdi\n", registerMap[reg1]);
                        printf("\tcall outchar\n");
                        printf("\tpop %%%s\n", registerMap[5]);
                        break;

                    case I_BR:
                        fread(buffer, 1, 2, file);
                        label = convertToShort(buffer);
                        if ((f_flag & 0x0001) == 0x0001) {
                            printf("\tjmp %d\n", label);
                        }

                        break;

                    case I_JR:
                        fread(buffer, 1, 2, file);
                        label = convertToShort(buffer);
                        if ((f_flag & 0x0001) == 0x0001) {
                            printf("\tjmp %d\n", label);
                        }
                        break;
                }
                break;

            case 2:     // 2-operand instruction
                switch ((instruction >> 8) & 0xFF) {
                    case I_ADD:
                        printf("\tadd ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("%%%s, %%%s\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_SUB:
                        printf("\tsub ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("%%%s, %%%s\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_MUL:
                        printf("\timul ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("%%%s, %%%s\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_AND:
                        printf("\tand ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("%%%s, %%%s\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_OR:
                        printf("\tor ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("%%%s, %%%s\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_XOR:
                        printf("\txor ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("%%%s, %%%s\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_TEST:
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);

                        if ((reg1 & reg2) != 0) {
                            f_flag = (f_flag | 0x0001);
                        } else {
                            f_flag = (f_flag & 0xFFFE);
                        }
                        printf("\tmov $%d, %%%s\n", f_flag, registerMap[13]);
                        break;

                    case I_CMP:
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        if (reg1 < reg2) {
                            f_flag = (f_flag | 0x0001);
                        } else {
                            f_flag = (f_flag & 0xFFFE);
                        }
                        printf("\tmov $%d, %%%s\n", f_flag, registerMap[13]);
                        break;

                    case I_EQU:
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);

                        if (reg1 == reg2) {
                            f_flag = (f_flag | 0x0001);
                        } else {
                            f_flag = (f_flag & 0xFFFE);
                        }
                        printf("\tmov $%d, %%%s\n", f_flag, registerMap[13]);
                        break;

                    case I_MOV:
                        printf("\tmov ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("%%%s, %%%s\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_LOAD:
                        printf("\tmov ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("(%%%s), %%%s\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_STOR:
                        printf("\tmov ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("%%%s, (%%%s)\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_LOADB:
                        printf("\tmov ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("(%%%s), %%%s\n", registerMap[reg1], registerMap[reg2]);
                        break;

                    case I_STORB:
                        printf("\tmov ");
                        reg1 = ((instruction >> 4) & 0xF);
                        reg2 = ((instruction) & 0xF);
                        printf("%%%s, (%%%s)\n", registerMap[reg1], registerMap[reg2]);
                        break;
                }
                break;

            case 3:     // extended instruction
                switch ((instruction >> 8) & 0xFF) {
                    case I_JMP:
                        fread(buffer, 1, 2, file);
                        label = convertToShort(buffer);
                        printf("\tjmp .L%04X\n", label);
                        break;

                    case I_CALL:
                        fread(buffer, 1, 2, file);
                        label = convertToShort(buffer);
                        printf("\tcall %d\n", label);
                        break;

                    case I_LOADI:
                        fread(buffer, 1, 2, file);
                        unsigned short imm_val = convertToShort(buffer);
                        short reg = ((instruction >> 4) & 0xF);
                        printf("\tmov $%d, %%%s\n", imm_val, registerMap[reg]);
                        break;
                }
                break;
        }
    }

    //close debug mode if cld not called at the end of program
    if (debug_flag == 1) {
        printf("\tcall debug\n");
        debug_flag = 0;
    }
}