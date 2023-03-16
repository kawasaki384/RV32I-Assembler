#ifndef HEADER_H_
#define HEADER_H_

typedef struct _vector
{
    unsigned int x;
    unsigned int y;
} vector;

typedef struct _instruction_info
{
    unsigned int opcode;
    unsigned int funct3;
    unsigned int funct7;
    int imm; //負の数の場合もある
    unsigned int rd;
    unsigned int rs1;
    unsigned int rs2;
    unsigned int shamt;
    char type;

    _Bool is_imm_label;
    
    /*
    immはlabelなのか
    is_imm_label:0 immはlabelではない
    is_imm_label:1 immはlabelである
    */
}instruction_info;

typedef struct _instruction{
    instruction_info info;
    int status;
} instruction;

/*
status = 0の時、正常
status = 1の時、set_instruction_detail関数中のエラー
*/


/*******instruction.c*********/
unsigned int init_label(unsigned int y, int *addr);
void _0flag();
instruction set_instruction_detail(char* instruction_name,unsigned int y);

/*******main.c*******/
extern char* token[128][8];

/*******code.c*******/
unsigned int getcode(unsigned int y,int *addr);
// BRANCH系に必要なOperandを代入します。
void set_opcode_BRANCH(instruction_info *inst, unsigned int y);

// LOAD系に必要なOperandを代入します。
void set_opcode_LOAD(instruction_info *inst, unsigned int y);

// STORE系に必要なOperandを代入します。
void set_opcode_STORE(instruction_info *inst, unsigned int y);

// OP_IMM系に必要なOperandを代入します。
void set_opcode_OP_IMM(instruction_info *inst, unsigned int y);

// OP系に必要なOperandを代入します。
void set_opcode_OP(instruction_info *inst, unsigned int y);

// その他に必要なOperandを代入します。
void set_opcode_OTHER(instruction_info *inst, unsigned int y);

extern const char *instruction_list[];

#endif /*HEADER_H_*/
