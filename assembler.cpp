#include "SoftProcessor.h"
#include "assembler.h"

int main()
{
    FILE *f = my_fopen("source.asm", "r");

    int size = 0;
    int *code = TranslateToByteCode(f, &size);

    fclose(f);

    f = my_fopen("byte_code.bin", "w");

    WriteByteCode(f, code, size);

    fclose(f);

}

FILE *my_fopen(const char *filename, const char *type)
{
    assert(filename);
    assert(type);

    FILE *f = fopen(filename, type);
    if (f == NULL)
    {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return nullptr;
    }
    return f;
}

int *TranslateToByteCode(FILE *f, int *code_size)
{
    assert(f);
    assert(code_size);

    int *code = (int *)calloc(START_CODE_SIZE, sizeof(int));
    if (code == NULL)
    {
        return 0;
    }

    int size = START_CODE_SIZE;
    int pos = 1;
    int line = 1;
    code[0] = COMMANDS_VERSION;
    int val = 0;
    char reg_name[8];

    char cmdstr[MAX_COMMAND_SIZE] = {};
    while (fgets(cmdstr, sizeof(cmdstr), f) != NULL)
    {
        char *comment = strchr(cmdstr, ';');
        if(comment != NULL)
        {
            *comment = '\0';
        }

        char cmd[MAX_COMMAND_SIZE];
        int n_scanned = sscanf(cmdstr, "%s", cmd);
        if (n_scanned <= 0)
        {
            continue;
        }

        if(pos > size - 2)
        {
            int *new_code = (int *)realloc(code, size * 2 * sizeof(int))
            if (code == NULL)
            {
                return 0;
            }
            code = new_code;
            size *= 2;
        }

        if (strcmp(cmd, "ADD") == 0)
        {
            code[pos++] = CMD_ADD;
        }

        else if (strcmp(cmd, "PUSH") == 0)
        {
            sscanf(cmdstr, "%*s %d", &val);
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

        else if (strcmp(cmd, "PUSHREG") == 0)
        {
            sscanf(cmdstr, "%*s %s", reg_name);
            code[pos++] = CMD_PUSHREG;
            code[pos++] = GetRegNumber(reg_name);
        }

        else if (strcmp(cmd, "POPREG") == 0)
        {
            sscanf(cmdstr, "%*s %s", reg_name);
            code[pos++] = CMD_POPREG;
            code[pos++] = GetRegNumber(reg_name);
        }

        else if (strcmp(cmd, "MUL") == 0)
        {
            code[pos++] = CMD_MUL;
        }

        else if (strcmp(cmd, "SUB") == 0)
        {
            code[pos++] = CMD_SUB;
        }

        else
        {
            printf("Syntax error! source.asm:%d\n", line);
        }

        line++;
    }

    *code_size = pos;
    code = (int *)realloc(code, pos * sizeof(int));

    return code;
}

int GetRegNumber(const char *reg_name)
{
    if (strcmp(reg_name, "AX") == 0)
    {
        return 0;
    }

    else if (strcmp(reg_name, "BX") == 0)
    {
        return 1;
    }

    else if (strcmp(reg_name, "CX") == 0)
    {
        return 2;
    }

    else if (strcmp(reg_name, "DX") == 0)
    {
        return 3;
    }

    else if (strcmp(reg_name, "EX") == 0)
    {
        return 4;
    }

    else if (strcmp(reg_name, "FX") == 0)
    {
        return 5;
    }

    else if (strcmp(reg_name, "GX") == 0)
    {
        return 6;
    }

    printf("Invalid register name!");

    return -1;
}

int WriteByteCode(FILE *f, int *code, int size)
{
    assert(f);
    assert(code);

    for (int i = 0; i < size - 1; i++)
    {
        fprintf(f, "%d ", code[i]);
    }

    fprintf(f, "%d", code[size - 1]);

    return 1;
}
