#include "header.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _label_or_instruction
{
    _Bool flag;
    /*
    flag:0 label
    flag:1 instruction(label以外のもの)
    */
    char *value;
} label_or_instruction;

typedef struct _label
{
    char *name;
    int address;
} label;

label labels[128];
int n = 0;

label_or_instruction get_label_or_instruction(unsigned int i)
{
    label_or_instruction return_value;
    /*********************************************************************/
    // エラー処理
    if (token[i][0] == NULL || token[i][0] == 0)
    {
        return_value.value = NULL;
        return return_value;
    }
    /*********************************************************************/

    if (token[i][0][strlen(token[i][0]) - 1] == ':')
        return_value.flag = 0;
    else
        return_value.flag = 1;

    return_value.value = token[i][0];

    return return_value;
}

unsigned int char_to_register(char *string)
{
    unsigned int value = 0;
    sscanf(string, "x%d", &value);
    return value;
}

// ディスプレースメントからレジスタをとる
unsigned int displacement_to_register(char *string)
{
    // 一応トークンの最大値
    char value[128];
    char dummy[128];

    unsigned int reg;
    if (sscanf(string, "%[^a-(]%s(x%s)", dummy, value) != 2)
    {
        printf("displacement_to_register関数のエラー");
    }

    sscanf(value, "(x%d)", &reg);

    return reg;
}

typedef struct _char_to_imm_return
{
    int imm;
    _Bool is_imm_label;
} char_to_imm_return;

char_to_imm_return char_to_imm(char *string)
{
    char_to_imm_return return_value;
    return_value.is_imm_label = 0; // もしimmがlabelだったら後に1に書き換える。
    int calc = 0;
    _Bool flag = 0;

    unsigned int count = 0;

    char reg_dummy[128];
    char imm_sscanf[128];

    unsigned base = 10;

    // 0x???の場合
    if (strlen(string) > 2 && string[0] == '0' && string[1] == 'x')
    {
        base = 16;
        for (int i = 2; i < strlen(string); i++)
        {
            string[i - 2] = string[i];
        }
        string[strlen(string) - 2] = '\0';
    }
    // 0b???の場合
    else if (strlen(string) > 2 && string[0] == '0' && string[1] == 'b')
    {
        base = 2;
        for (int i = 2; i < strlen(string); i++)
        {
            string[i - 2] = string[i];
        }
        string[strlen(string) - 2] = '\0';
    }

    for (int i = 0; i < strlen(string); i++)
    {
        int a = (int)pow(base, strlen(string) - 1 - i);
        switch (string[i])
        {
        case '-':
            if (i == 0)
                flag = 1;
            else
                printf("-が不適切な位置にあります。");
            break;
        case '0':
            calc += a * 0;
            break;
        case '1':
            calc += a * 1;
            break;
        case '2':
            calc += a * 2;
            break;
        case '3':
            calc += a * 3;
            break;
        case '4':
            calc += a * 4;
            break;
        case '5':
            calc += a * 5;
            break;
        case '6':
            calc += a * 6;
            break;
        case '7':
            calc += a * 7;
            break;
        case '8':
            calc += a * 8;
            break;
        case '9':
            calc += a * 9;
            break;
        case 'a':
            calc += a * 10;
            break;
        case 'b':
            calc += a * 11;
            break;
        case 'c':
            calc += a * 12;
            break;
        case 'd':
            calc += a * 13;
            break;
        case 'e':
            calc += a * 14;
            break;
        case 'f':
            calc += a * 15;
            break;
        case 'A':
            calc += a * 10;
            break;
        case 'B':
            calc += a * 11;
            break;
        case 'C':
            calc += a * 12;
            break;
        case 'D':
            calc += a * 13;
            break;
        case 'E':
            calc += a * 14;
            break;
        case 'F':
            calc += a * 15;
            break;
        default:
            count++;
        }
    }

    _Bool c = 0;
    if (count > 0)
    {
        for (int i = 0; i < sizeof(labels) / sizeof(label); i++)
        {
            char format[1024];
            sprintf(format, "%s:", string);
            if (labels[i].name != NULL && !strcmp(format, labels[i].name))
            {
                c = 1;
                return_value.is_imm_label = 1;
                fflush(stdout);
                calc = labels[i].address;
                break;
            }
        }
        if (c == 0)
        {
            printf("存在しないラベル:%s", string);
        }
    }

    if (flag == 1)
        calc *= -1;

    return_value.imm = calc;

    if (calc > 0b111111111111)
    {
        printf("OverFlow Warning!! \n");
    }

    return return_value;
}

// ディスプレースメントからイミーディエイトをとる
char_to_imm_return displacement_to_imm(char *string)
{
    // 一応トークンの最大値
    char value[128];
    char dummy[128];

    unsigned int reg;
    if (sscanf(string, "%[^a-(]%s(x%s)", value, dummy) != 2)
    {
        printf("displacement_to_imm関数のエラー");
    }

    return char_to_imm(value);
}

// offset(rs1)のoffsetを取得します。 エラー処理未実装
char *get_offset_(char *str)
{
    char *offset;
    char buff[50];
    sscanf(str, "%s(%s)", offset, buff);
    return offset;
}

// offset(rs1)のrs1を取得します。　エラー処理未実装
char *get_register_(char *str)
{
    char *rs1;
    char buff[50];
    sscanf(str, "%s(%s)", buff, rs1);
    return rs1;
}

// BRANCH系に必要なOperandを代入します。
void set_opcode_BRANCH(instruction_info *inst, unsigned int y)
{
    if (token[y][1] != NULL && token[y][2] != NULL)
    {
        inst->rs1 = char_to_register(token[y][1]);
        inst->rs2 = char_to_register(token[y][2]);
        inst->imm = char_to_imm(token[y][3]).imm;
        inst->is_imm_label = char_to_imm(token[y][3]).is_imm_label;
    }
    else
    {
        printf("Error_set_opcode_branch Operandが少なすぎます 行:%d \n", y);
    }
}

// LOAD系に必要なOperandを代入します。
void set_opcode_LOAD(instruction_info *inst, unsigned int y)
{
    if (token[y][1] != NULL && token[y][2] != NULL && token[y][3] != NULL)
    {
        inst->rd = char_to_register(token[y][1]);
        inst->imm = char_to_imm(token[y][3]).imm;
        inst->rs1 = char_to_register(token[y][2]);
        inst->is_imm_label = char_to_imm(token[y][3]).is_imm_label;
    }
    else if (token[y][1] != NULL && token[y][2] != NULL)
    {
        inst->rd = char_to_register(token[y][1]);
        inst->imm = displacement_to_imm(token[y][2]).imm;
        inst->rs1 = displacement_to_register(token[y][2]);
        inst->is_imm_label = displacement_to_imm(token[y][2]).is_imm_label;
    }
    else
    {
        printf("Error_set_opcode_load Operandが少なすぎます 行:%d \n", y);
    }
}

// STORE系に必要なOperandを代入します。
void set_opcode_STORE(instruction_info *inst, unsigned int y)
{
    if (token[y][1] != NULL && token[y][2] != NULL && token[y][3] != NULL)
    {
        inst->rs2 = char_to_register(token[y][1]);
        inst->rs1 = char_to_register(token[y][2]);
        inst->imm = char_to_imm(token[y][3]).imm;
        inst->is_imm_label = char_to_imm(token[y][3]).is_imm_label;
    }
    else if (token[y][1] != NULL && token[y][2] != NULL)
    {
        // ディスプレースメント付きレジスタ間接指定の場合
        inst->rs2 = char_to_register(token[y][1]);
        inst->rs1 = displacement_to_register(token[y][2]);
        inst->imm = displacement_to_imm(token[y][2]).imm;
        inst->is_imm_label = displacement_to_imm(token[y][2]).is_imm_label;
    }
    else
    {
        printf("Error_set_opcode_store Operandが少なすぎます 行:%d \n", y);
    }
}

// OP_IMM系に必要なOperandを代入します。
void set_opcode_OP_IMM(instruction_info *inst, unsigned int y)
{
    if (token[y][1] != NULL && token[y][2] != NULL && token[y][3] != NULL)
    {
        inst->rd = char_to_register(token[y][1]);
        inst->rs1 = char_to_register(token[y][2]);
        inst->imm = char_to_imm(token[y][3]).imm;
        inst->is_imm_label = char_to_imm(token[y][3]).is_imm_label;
    }
    else if (token[y][1] != NULL && token[y][2] != NULL)
    {
        // ディスプレースメント付きレジスタ間接指定の場合もある。
        inst->rd = char_to_register(token[y][1]);
        inst->rs1 = displacement_to_register(token[y][2]);
        inst->imm = displacement_to_imm(token[y][2]).imm;
        inst->is_imm_label = displacement_to_imm(token[y][2]).is_imm_label;
    }
    else
    {
        printf("Error_set_opcode_op_imm Operandが少なすぎます 行:%d \n", y);
    }
}

// OP系に必要なOperandを代入します。
void set_opcode_OP(instruction_info *inst, unsigned int y)
{
    if (token[y][1] != NULL && token[y][2] != NULL && token[y][3] != NULL)
    {
        inst->rd = char_to_register(token[y][1]);
        inst->rs1 = char_to_register(token[y][2]);
        inst->rs2 = char_to_register(token[y][3]);
    }
    else
    {
        printf("Error_set_opcode_op Operandが少なすぎます 行:%d \n", y);
    }
}

// その他に必要なOperandを代入します。
void set_opcode_OTHER(instruction_info *inst, unsigned int y)
{
    if (inst->opcode == 0b0110111)
    {
        // LUI命令
        if (token[y][1] != NULL && token[y][2] != NULL)
        {
            inst->rd = char_to_register(token[y][1]);
            inst->imm = char_to_imm(token[y][2]).imm;
            inst->is_imm_label = char_to_imm(token[y][2]).is_imm_label;
        }
    }
    else if (inst->opcode == 0b0010111)
    {
        // AUIPC命令
        if (token[y][1] != NULL && token[y][2] != NULL)
        {
            inst->rd = char_to_register(token[y][1]);
            inst->imm = char_to_imm(token[y][2]).imm;
            inst->is_imm_label = char_to_imm(token[y][2]).is_imm_label;
        }
    }
    else if (inst->opcode == 0b1101111)
    {
        // JAL命令
        if (token[y][1] != NULL && token[y][2] != NULL)
        {
            inst->rd = char_to_register(token[y][1]);
            inst->imm = char_to_imm(token[y][2]).imm;
            inst->is_imm_label = char_to_imm(token[y][2]).is_imm_label;
        }
    }
    else if (inst->opcode == 0b1100111)
    {
        // JALR命令
        if (token[y][1] != NULL && token[y][2] != NULL && token[y][3] != NULL)
        {
            inst->rd = char_to_register(token[y][1]);
            inst->rs1 = char_to_register(token[y][2]);
            inst->imm = char_to_imm(token[y][3]).imm;
            inst->is_imm_label = char_to_imm(token[y][3]).is_imm_label;
        }
    }
}

// オペランド、命令の情報からタイプBの機械語を生成します。
unsigned int type_B(unsigned int opcode, int imm, unsigned int funct3, unsigned int rs1, unsigned int rs2)
{
    unsigned int output = 0;
    output |= (opcode & 0b1111111);
    output |= ((imm >> 11) & 0b1) << 7;
    output |= ((imm >> 1) & 0b1111) << 8;
    output |= (funct3 & 0b111) << 12;
    output |= (rs1 & 0b11111) << 15;
    output |= (rs2 & 0b11111) << 20;
    output |= ((imm >> 5) & 0b111111) << 25;
    output |= ((imm >> 12) & 0b1) << 31;

    return output;
}

// オペランド、命令の情報からタイプRの機械語を生成します。
unsigned int type_R(unsigned int opcode, unsigned int rd, unsigned int funct3, unsigned int rs1, unsigned int rs2, unsigned int funct7)
{
    unsigned int output = 0;
    output |= (opcode & 0b1111111);
    output |= (rd & 0b11111) << 7;
    output |= (funct3 & 0b111) << 12;
    output |= (rs1 & 0b11111) << 15;
    output |= (rs2 & 0b11111) << 20;
    output |= (funct7 & 0b1111111) << 25;

    return output;
}

// オペランド、命令の情報からタイプIの機械語を生成します。
unsigned int type_I(unsigned int opcode, unsigned int rd, unsigned int funct3, unsigned int rs1, int imm)
{
    unsigned int output = 0;
    output |= (opcode & 0b1111111);
    output |= (rd & 0b11111) << 7;
    output |= (funct3 & 0b111) << 12;
    output |= (rs1 & 0b11111) << 15;
    output |= (imm & 0b111111111111) << 20;

    return output;
}

// オペランド、命令の情報からタイプSの機械語を生成します。
unsigned int type_S(unsigned int opcode, unsigned int funct3, unsigned int rs1, unsigned int rs2, int imm)
{
    printf("%d %d %d", rs1, rs2, imm);
    unsigned int output = 0;
    output |= (opcode & 0b1111111);
    output |= (imm & 0b11111) << 7;
    output |= (funct3 & 0b111) << 12;
    output |= (rs1 & 0b11111) << 15;
    output |= (rs2 & 0b11111) << 20;
    output |= ((imm >> 5) & 0b1111111) << 25;

    return output;
}

// オペランド、命令の情報からタイプUの機械語を生成します。
unsigned int type_U(unsigned int opcode, unsigned int rd, int imm)
{
    unsigned int output = 0;
    output |= (opcode & 0b1111111);
    output |= (rd & 0b11111) << 7;
    output |= (imm & 0b11111111111111111111) << 12;

    return output;
}

// オペランド、命令の情報からタイプJの機械語を生成します。
unsigned int type_J(unsigned int opcode, unsigned int rd, int imm)
{
    unsigned int output = 0;
    output |= (opcode & 0b1111111);
    output |= (rd & 0b11111) << 7;
    output |= ((imm >> 12) & 0b11111111) << 12;
    output |= ((imm >> 11) & 0b1) << 20;
    output |= ((imm >> 1) & 0b1111111111) << 21;
    output |= ((imm >> 20) & 0b1) << 31;

    return output;
}

// オペランド、命令の情報からタイプI(5bit)の機械語を生成します。
unsigned int type_5(unsigned int opcode, unsigned int rd, unsigned int funct3, unsigned int rs1, unsigned int shamt, unsigned int funct7)
{
    unsigned int output = 0;
    output |= (opcode & 0b1111111);
    output |= (rd & 0b11111) << 7;
    output |= (funct3 & 0b111) << 12;
    output |= (rs1 & 0b11111) << 15;
    output |= (shamt & 0b11111) << 20;
    output |= (funct7 & 0b1111111) << 25;

    return output;
}

_Bool flag = 0;

unsigned int init_label(unsigned int y, int *addr)
{
    if (flag == 1)
    {
        *addr -= 4;
        flag = 0;
    }

    label_or_instruction _value = get_label_or_instruction(y);

    if (_value.flag == 0 && _value.value != NULL)
    {
        printf("新しいラベル:%s %x\n", _value.value, *addr);

        labels[n].name = _value.value;
        labels[n].address = *addr;
        flag = 1;
        n++;
    }

    if (_value.value == NULL)
        return 1; // エラーの場合1を返す。
    else
        return 0; // エラー出ない場合0を返す。
}

void _0flag()
{
    flag = 0;
}

unsigned int getcode(unsigned int y, int *addr)
{
    unsigned int output = 0;

    label_or_instruction _value = get_label_or_instruction(y);

    if (_value.value == NULL)
    {
        // 空白や行全体がコメントアウトの時に0xffffffffを返す
        // もちろん0xffffffffはrisc-vの既存の命令セットでは表すことができないため、エラーとすることができる。
        return 0xffffffff;
    }

    if (flag == 1)
    {
        *addr -= 4;
        flag = 0;
    }

    if (_value.flag == 0)
    {
        flag = 1;
    }
    if (_value.flag == 1)
    {
        // 命令とOperandの詳細を設定
        instruction inst = set_instruction_detail(_value.value, y);

        if (inst.status == 1)
        {
            // エラー処理、_value.valueが命令ではない
        }

        switch (inst.info.type)
        {
        case 'B':
            if (inst.info.is_imm_label == 1)
            { // immがラベルの場合、immからaddressを引く必要がある
                output = type_B(inst.info.opcode, inst.info.imm - *addr, inst.info.funct3, inst.info.rs1, inst.info.rs2);
            }
            else
            { // immがラベルでない場合、immをそのままにする必要がある。
                output = type_B(inst.info.opcode, inst.info.imm, inst.info.funct3, inst.info.rs1, inst.info.rs2);
            }
            break;
        case 'R':
            output = type_R(inst.info.opcode, inst.info.rd, inst.info.funct3, inst.info.rs1, inst.info.rs2, inst.info.funct7);
            break;
        case 'I':
            output = type_I(inst.info.opcode, inst.info.rd, inst.info.funct3, inst.info.rs1, inst.info.imm);
            break;
        case 'S':
            output = type_S(inst.info.opcode, inst.info.funct3, inst.info.rs1, inst.info.rs2, inst.info.imm);
            break;
        case 'U':
            output = type_U(inst.info.opcode, inst.info.rd, inst.info.imm);
            break;
        case 'J':
            if (inst.info.is_imm_label == 1)
            { // immがラベルの場合、immからaddressを引く必要がある
                output = type_J(inst.info.opcode, inst.info.rd, inst.info.imm - *addr);
            }
            else
            { // immがラベルでない場合、immをそのままにする必要がある。
                output = type_J(inst.info.opcode, inst.info.rd, inst.info.imm);
            }
            break;
        case '5':
            output = type_5(inst.info.opcode, inst.info.rd, inst.info.funct3, inst.info.rs1, inst.info.shamt, inst.info.funct7);
            break;
        }
    }

    return output;
}
