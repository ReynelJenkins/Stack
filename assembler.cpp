#include "SoftProcessor.h"
#include "assembler.h"
#include "file.h"

int main(int argc, char *argv[])
{
    const char *source_file_name = "source.asm";

    if (argc > 1)
    {
        source_file_name = argv[1];
    }

    FILE *file = my_fopen(source_file_name, "r");

    int size = 0;
    int *code = TranslateToByteCode(file, &size);

    if (second_run_required == 1)
    {
        rewind(file);

        code = TranslateToByteCode(file, &size);
    }

    fclose(file);

    file = my_fopen("byte_code.bin", "wb");

    WriteByteCode(file, code, size);

    fclose(file);

}

int *TranslateToByteCode(FILE *f, int *code_size)
{
    assert(f);
    assert(code_size);

    struct AssemblerValues asm_vals = {};
    asm_vals.code = (int *)calloc(START_CODE_SIZE, sizeof(int));
    if (asm_vals.code == nullptr)
    {
        return 0;
    }

    while (fgets(asm_vals.cmdstr, sizeof(asm_vals.cmdstr), f) != NULL)
    {
        asm_vals.line++;
        char *comment = strchr(asm_vals.cmdstr, ';');

        if(comment != NULL)
        {
            *comment = '\0';
        }

        char cmd[MAX_COMMAND_SIZE];
        int n_scanned = sscanf(asm_vals.cmdstr, "%s", cmd);

        if (n_scanned <= 0)
        {
            continue;
        }

        if (cmd[0] == ':')
        {
            if (label_count >= MAX_LABELS)
            {
                printf("Too many labels!\n");
                return 0;
            }

            strcpy(labels[label_count].name, cmd + 1);
            labels[label_count].addr = asm_vals.pos;
            label_count++;

            continue;
        }

        if(asm_vals.pos > asm_vals.size - 2)
        {
            int *new_code = (int *)realloc(asm_vals.code, asm_vals.size * 2 * sizeof(int));
            if (new_code == NULL)
            {
                return 0;
            }
            asm_vals.code = new_code;
            asm_vals.size *= 2;
        }

        int found = 0;
        for (int i = 0; i < commands_count; i++)
        {
            if (strcmp(commands[i].name, cmd) == 0)
            {
                found = 1;
                asm_vals.code[asm_vals.pos++] = commands[i].command_code;
                int error = GetArgument(&asm_vals, commands[i]);

                if (error)
                {
                    return NULL;
                }

                break;
            }
        }

        if (!found)
        {
            printf("Unknown command '%s' (line %d)\n", cmd, asm_vals.line);
            free(asm_vals.code);

            return NULL;
        }
    }

    pass++;

    *code_size = asm_vals.pos;

    return asm_vals.code;
}

int GetArgument(struct AssemblerValues *vals, struct Command command)
{
    int val = 0;

    switch (command.has_arg)
    {
        case ARG_TYPE_VALUE:
        {
            if (sscanf(vals->cmdstr, "%*s %d", &val) != 1)
            {
                printf("Invalid %s (line %d)\n", command.arg_type, vals->line);
                free(vals->code);

                return 1;
            }
            vals->code[(vals->pos)++] = val;

            break;
        }

        case ARG_TYPE_REGISTER:
        {
            if (sscanf(vals->cmdstr, "%*s %s", vals->reg_name) != 1)
            {
                printf("Invalid %s (line %d)\n", command.arg_type, vals->line);
                free(vals->code);

                return 1;
            }

            int reg_num = GetRegNumber(vals->reg_name);

            if (reg_num == -1)
            {
                printf("Invalid register name '%s' (line %d)\n", vals->reg_name, vals->line);
                free(vals->code);

                return 1;
            }

            vals->code[(vals->pos)++] = reg_num;

            break;
        }

        case ARG_TYPE_LABEL_NUMBER:
        {
            char arg[32] = {};

            if (sscanf(vals->cmdstr, "%*s %s", arg) != 1)
            {
                printf("Invalid %s '%s' (line %d)\n", command.arg_type, arg, vals->line);
                free(vals->code);

                return 1;
            }

            if (arg[0] == ':')
            {
                if((vals->code[(vals->pos)++] = GetLabelPos(arg + 1)) == -2)
                {
                    printf("Unknown label '%s' (line %d)\n", arg, vals->line);

                    return 1;
                }
            }

            else
            {
                if (sscanf(arg, "%d", &val) != 1)
                {
                    printf("Invalid %s '%s' (line %d)\n", command.arg_type, arg, vals->line);
                    free(vals->code);

                    return 1;
                }

                vals->code[(vals->pos)++] = val;
            }

            break;
        }

        case ARG_TYPE_NO_ARG:
        default:
            break;
    }

    return 0;
}

int GetLabelPos(const char *label)
{
    for (int i = 0; i < label_count; i++)
    {
        if (strcmp(labels[i].name, label) == 0)
        {
            return labels[i].addr;
        }
    }

    if(pass == 2)
    {
        return -2;
    }

    second_run_required = 1;

    return -1;
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

    return -1;
}

int WriteByteCode(FILE *file, int *code, int size)
{
    assert(file);

    if (code == nullptr)
    {
        printf("Failed to compile!");
        return 1;
    }

    fwrite(&SIGN, sizeof(int), 1, file);

    fwrite(&COMMANDS_VERSION, sizeof(int), 1, file);

    fwrite(&size, sizeof(int), 1, file);

    fwrite(code, sizeof(int), size, file);

    return 1;
}
