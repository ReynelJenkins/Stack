#include "SoftProcessor.h"

const int MAX_TOTAL_SIZE = 1000;
const int MAX_COMMAND_SIZE = 256;

int main()
{
    FILE *f = fopen("source.asm", "r");
    if (f == NULL)
    {
        printf("Ошибка: не удалось открыть файл source.asm\n");
        return 1;
    }

    int code[MAX_TOTAL_SIZE] = {};
    unsigned int pos = 0;
    int line = 1;

    char cmdstr[MAX_COMMAND_SIZE] = {};
    while (fgets(cmdstr, sizeof(cmdstr), f) != NULL)
    {
        char cmd[MAX_COMMAND_SIZE];
        int val = 0;
        sscanf(cmdstr, "%s %d", cmd, &val);

        if (strcmp(cmd, "ADD") == 0)
        {
            code[pos++] = CMD_ADD;
        }

        else if (strcmp(cmd, "PUSH") == 0)
        {
            code[pos++] = CMD_PUSH;
            code[pos++] = val;
        }

        else if (strcmp(cmd, "DIV") == 0)
        {
            code[pos++] = CMD_DIV;
        }

        else if (strcmp(cmd, "OUT") == 0)
        {
            code[pos++] = CMD_OUT;
        }

        else if (strcmp(cmd, "HLT") == 0)
        {
            code[pos++] = CMD_HLT;
        }

        else
        {
            printf("Syntax error! source.asm:%d\n", line);
        }
        line++;
    }

    for (int i = 0; i < MAX_TOTAL_SIZE; i++)
    {
        printf("%d ", code[i]);
    }

    fclose(f);
    return 0;
}
