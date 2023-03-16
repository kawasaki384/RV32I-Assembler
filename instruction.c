#include "header.h"
#include <string.h>
#include <stdio.h>

//#define DEBUG
//コメントアウトを外すとデバッグモードになります。//

// 定義されていない値
#define UNDEFINED_DETAIL -3

const char *instruction_list[] = {"LUI", "AUIPC", "JAL", "JALR", "BEQ",
                                  "BNE", "BLT", "BGE", "BLTU", "BGEU",
                                  "LB", "LH", "LW", "LBU", "LHU", "SB", "SH",
                                  "SW", "ADDI", "SLTI", "SLTIU", "XORI", "ORI",
                                  "ANDI", "SLLI", "SRLI", "SRAI", "ADD", "SUB",
                                  "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR",
                                  "AND",
                                  "lui", "auipc", "jal", "jalr", "beq",
                                  "bne", "blt", "bge", "bltu", "bgeu",
                                  "lb", "lh", "lw", "lbu", "lhu", "sb", "sh",
                                  "sw", "addi", "slti", "sltiu", "xori", "ori",
                                  "andi", "slli", "srli", "srai", "add", "sub",
                                  "sll", "slt", "sltu", "xor", "srl", "sra", "or",
                                  "and"};

const char *instructions_BRANCH[] = {"BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU",
                                     "beq", "bne", "blt", "bge", "bltu", "bgeu"};

const unsigned int instructions_BRANCH_funct3[] = {0b000, 0b001, 0b100, 0b101, 0b110, 0b111,
                                                   0b000, 0b001, 0b100, 0b101, 0b110, 0b111};

const char *instructions_LOAD[] = {"LB", "LH", "LW", "LBU", "LHU",
                                   "lb", "lh", "lw", "lbu", "lhu"};

const unsigned int instructions_LOAD_funct3[] = {0b000, 0b001, 0b010, 0b100, 0b101, 0b000, 0b001, 0b010, 0b100, 0b101};

const char *instructions_STORE[] = {"SB", "SH", "SW", "sb", "sh", "sw"};

const unsigned int instructions_STORE_funct3[] = {0b000, 0b001, 0b010, 0b000, 0b001, 0b010};

const char *instructions_OP_IMM[] = {"ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI",
                                     "addi", "slti", "sltiu", "xori", "ori", "andi", "slli", "srli", "srai"};

const unsigned int instructions_OP_IMM_funct3[] = {0b000, 0b010, 0b011, 0b100, 0b110, 0b111, 0b001, 0b101, 0b101,
                                                   0b000, 0b010, 0b011, 0b100, 0b110, 0b111, 0b001, 0b101, 0b101};

const unsigned int instructions_OP_IMM_funct7[] = {UNDEFINED_DETAIL, UNDEFINED_DETAIL, UNDEFINED_DETAIL, UNDEFINED_DETAIL, UNDEFINED_DETAIL, UNDEFINED_DETAIL,
                                                   0b0000000, 0b0000000, 0b0100000,
                                                   UNDEFINED_DETAIL, UNDEFINED_DETAIL, UNDEFINED_DETAIL, UNDEFINED_DETAIL, UNDEFINED_DETAIL, UNDEFINED_DETAIL,
                                                   0b0000000, 0b0000000, 0b0100000};

const char *instructions_OP[] = {"ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND",
                                 "add", "sub", "sll", "slt", "sltu", "xor", "srl", "sra", "or", "and"};

const unsigned int instructions_OP_funct3[] = {0b000, 0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b101, 0b110, 0b111,
                                               0b000, 0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b101, 0b110, 0b111};

const unsigned int instructions_OP_funct7[] = {0b0000000, 0b0100000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0100000, 0b0000000, 0b0000000,
                                               0b0000000, 0b0100000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0100000, 0b0000000, 0b0000000};

const char *
    instructions_OTHERS[] = {"LUI", "AUIPC", "JAL", "JALR",
                             "lui", "auipc", "jal", "jalr"};

const char instructions_OTHERS_Type[] = {'U', 'U', 'J', 'I',
                                         'U', 'U', 'J', 'I'};

const unsigned int instructions_OTHERS_Opcode[] = {0b0110111,0b0010111,0b1101111,0b1100111,0b0110111,0b0010111,0b1101111,0b1100111};
const unsigned int instructions_OTHERS_funct3[] = {UNDEFINED_DETAIL,UNDEFINED_DETAIL,UNDEFINED_DETAIL,0b000,UNDEFINED_DETAIL,UNDEFINED_DETAIL,UNDEFINED_DETAIL,0b000};

#define instruction_list_len sizeof(instruction_list) / sizeof(char *)

//ブランチ系の命令かどうか検索して、ブランチ系の命令だった場合はinfoに詳細を代入
_Bool set_instructions_BRANCH(char* instruction_name,instruction_info *info){
    _Bool flag = 0;
    for (int i = 0; i < sizeof(instructions_BRANCH) / sizeof(char *); i++)
    {
        if (!strcmp(instruction_name, instructions_BRANCH[i]))
        {
            flag = 1;
            info->opcode = 0b1100011;
            info->type = 'B';
            info->funct3 = instructions_BRANCH_funct3[i];
        }
    }

    return flag;
}

//ロード系の命令かどうか検索して、ロード系の命令だった場合はinfoに詳細を代入
_Bool set_instructions_LOAD(char* instruction_name,instruction_info *info){
    _Bool flag = 0;
    for (int i = 0; i < sizeof(instructions_LOAD) / sizeof(char *); i++)
    {
        if (!strcmp(instruction_name, instructions_LOAD[i]))
        {
            flag = 1;
            info->opcode = 0b0000011;
            info->type = 'I';
            info->funct3 = instructions_LOAD_funct3[i];
        }
    }
    return flag;
}

//ストア系の命令かどうか検索して、ストア系の命令だった場合はinfoに詳細を代入
_Bool set_instructions_STORE(char *instruction_name,instruction_info *info){
    _Bool flag = 0;
    for (int i = 0; i < sizeof(instructions_STORE) / sizeof(char *); i++)
    {
        if (!strcmp(instruction_name, instructions_STORE[i]))
        {
            flag = 1;
            info->opcode = 0b0100011;
            info->type = 'S';
            info->funct3 = instructions_STORE_funct3[i];
        }
    }
    return flag;
}

//OP_IMM系の命令かどうか検索して、OP_IMM系の命令だった場合はinfoに詳細を代入
_Bool set_instructions_OP_IMM(char* instruction_name,instruction_info *info){
    _Bool flag = 0;
    for (int i = 0; i < sizeof(instructions_OP_IMM) / sizeof(char *); i++)
    {
        if (!strcmp(instruction_name, instructions_OP_IMM[i]))
        {
            flag = 1;
            info->opcode = 0b0010011;
            info->type = 'I';
            info->funct3 = instructions_OP_IMM_funct3[i];
            info->funct7 = instructions_OP_IMM_funct7[i];
            if (!strcmp(instruction_name, "andi") ||
                !strcmp(instruction_name, "slli") ||
                !strcmp(instruction_name, "srli") ||
                !strcmp(instruction_name, "srai"))
                info->type = '5'; // I(5bit)
        }
    }

    return flag;
}

//OP系の命令かどうか検索して、OP系の命令だった場合はinfoに詳細を代入
_Bool set_instructions_OP(char* instruction_name,instruction_info *info){
    _Bool flag = 0;
    for(int i = 0; i < sizeof(instructions_OP) / sizeof(char*);i++){
        if(!strcmp(instruction_name,instructions_OP[i])){
            flag = 1;
            info->opcode = 0b0110011;
            info->type = 'R';
            info->funct3 = instructions_OP_funct3[i];
            info->funct7 = instructions_OP_funct7[i];
        }
    }

    return flag;
}
 
//その他の命令かどうか検索して、その他の命令だった場合はinfoに詳細を代入
_Bool set_instructions_OTHERS(char* instruction_name,instruction_info *info){
    _Bool flag = 0;
    for (int i = 0; i < sizeof(instructions_OTHERS) / sizeof(char *); i++)
    {
        if (!strcmp(instruction_name, instructions_OTHERS[i]))
        {
            flag = 1;
            info->opcode = instructions_OTHERS_Opcode[i];
            info->type = instructions_OTHERS_Type[i];
            info->funct3 = instructions_OTHERS_funct3[i];
        }
    }

    return flag;
}

instruction set_instruction_detail(char *instruction_name,unsigned int y)
{
    _Bool flag = 0;
    int count = -1;

    instruction value;

    for (int i = 0; i < instruction_list_len; i++)
    {
        if (!strcmp(instruction_name, instruction_list[i]))
        {
            count = i, flag = 1;
            break;
        }
    }

    if(set_instructions_BRANCH(instruction_name,&value.info)){
        #ifdef DEBUG
        printf("This instruction is BRANCH \n");
        #endif
        set_opcode_BRANCH(&value.info,y);
    }
    else if(set_instructions_LOAD(instruction_name,&value.info)){
        #ifdef DEBUG
        printf("This instruction is LOAD \n");
        #endif
        set_opcode_LOAD(&value.info,y);
    }
    else if(set_instructions_STORE(instruction_name,&value.info)){
        #ifdef DEBUG
        printf("This instruction is STORE \n");
        #endif
        set_opcode_STORE(&value.info,y);
    }else if(set_instructions_OP_IMM(instruction_name,&value.info)){
        #ifdef DEBUG
        printf("This instruction is OP_IMM \n");
        #endif
        set_opcode_OP_IMM(&value.info,y);
    }else if(set_instructions_OP(instruction_name,&value.info)){
        #ifdef DEBUG
        printf("This instruction is OP %s\n",instruction_name);
        #endif
        set_opcode_OP(&value.info,y);
    }else if(set_instructions_OTHERS(instruction_name,&value.info)){
        #ifdef DEBUG
        printf("This instruction is OTHERS \n");
        #endif
        set_opcode_OTHER(&value.info,y);
    }else{
        printf("実装されていない命令です。 %s\n",instruction_name);
    }

    if (flag == 0)
    {
        printf("命令がありません。\n");
        value.status = 1;
    }else{
        value.status = 0;
    }

    return value;
}
