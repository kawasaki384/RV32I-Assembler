#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "header.h"

// プログラムを格納する配列です。[]の中身は最大の行数を表しています。
char *map[128];

// マップに格納されたプログラムが最小単位に分けられて格納されます。
char *token[128][8];
/*
char *token[128][32]の場合、トークンが128の行、32個の列で格納されていきます。
*/

void init_map()
{
    for (int i = 0; i < 128; i++)
    {
        map[i] = calloc(sizeof(char), 128);
        // if(map[i] == NULL)
        // エラー処理
    }
}

void free_map()
{
    for (int i = 0; i < 128; i++)
    {
        free(map[i]);
    }
}

vector set_map(FILE *file)
{
    /*
    マップにファイルの内容を書き込みます
    戻り値はファイルの行数(x),ファイルの最大の列数です。
    */

    vector value;

    int i = 0;
    while (fgets(map[i], 128, file) != NULL)
    {
        i++;
    }

    value.x = 128; // 列数の最大値
    value.y = i;   // 行数

    return value;
}

//\nを'\0'に変えます。
void trim_newline(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '\n')
            str[i] = '\0';
    }
}

void trim_commentout(char* str){
    for(int i = 0; i < strlen(str);i++)
    {
        if (str[i] == '#')
            str[i] = '\0';
    }
}

void set_token(vector vec)
{
    int j;
    for (int i = 0; i < vec.y; i++)
    {
        j = 0;
        trim_commentout(map[i]);
        char *pt = strtok(map[i], ",/ /\t");
        while (pt != NULL)
        {
            if (j == 8)
            {
                // トークンの数が多すぎます。
                printf("1行当たりのトークンの数が多すぎます。\n");
                fflush(stdout);
                abort();
            }

            trim_newline(pt);
            if (pt != NULL && pt[0] != 0)
                token[i][j] = pt, j++;

            pt = strtok(NULL, ",/ /\t");
        }
    }
}

int main(int argc, char **argv)
{
    init_map();
    FILE *file_input;
    file_input = fopen(argv[1], "r");
    if (file_input == NULL)
    {
        // エラー処理
        printf("file_input == NULL");
        return -1;
    }

    vector vec = set_map(file_input);

    set_token(vec);
    int addr = 0;

    for(int i = 0; i < vec.y;i++){
        if(init_label(i,&addr) == 0)
            addr+=4;
    }
    _0flag();
    addr = 0;
    FILE* output_file = fopen("a.out","w");
    for(int i = 0; i < vec.y;i++){
        unsigned int output = getcode(i,&addr);
        //outputが0(ラベルの場合)やエラー(0xffffffff)でない事を確かめてからprintf
        if(output != 0 && output != 0xffffffff){
        printf("%x:%x \n",addr,output);
        fwrite(&output,4,1,output_file);
        }else{
            //ラベル
        }
        //エラーじゃなければアドレスを進める
        if(output != 0xffffffff)
            addr+=4;
    }

    fclose(output_file);
    fclose(file_input);
    free_map();
}
