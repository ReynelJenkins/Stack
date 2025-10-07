#include "SoftProcessor.h"
#include "stack.h"
#include "processor.h"
#include "file.h"


//TODO: Fix verificator, spudump
int main()
{
    SPU my_spu;
    SPUCtor(&my_spu, START_STACK_SIZE);

    FILE *file = my_fopen(BYTE_CODE_SOURCE_FILE, "rb");
    LoadCodeFromFile(file, &my_spu);
    fclose(file);

    ExecuteCode(&my_spu);

    return 0;
}

enum SPUState SPUCtor(struct SPU *spu, int start_stack_size)
{
    StackCtor(&(spu->stk), start_stack_size);

    return SPU_OK;
}

enum SPUState SPUDtor(struct SPU *spu)
{
    VerifySPU(spu);
    free(spu->code);

    return SPU_DELETED;
}

enum SPUState LoadCodeFromFile(FILE *f, struct SPU *spu)
{
    int signature = 0;
    int version = 0;
    int code_count = 0;

    fread(&signature, sizeof(int), 1, f);
    fread(&version, sizeof(int), 1, f);
    fread(&code_count, sizeof(int), 1, f);

    int *file_code = (int *)calloc(code_count + HEADER_SIZE, sizeof(int));

    if(file_code == NULL)
    {
        printf("ERROR");
        return SPU_BAD_CODE;
    }
    file_code[0] = signature;
    file_code[1] = version;
    file_code[2] = code_count;

    fread(&file_code[HEADER_SIZE], sizeof(int), code_count, f);

    spu->code_size = code_count + HEADER_SIZE;

    spu->code = file_code;

    VerifySPU(spu);

    return SPU_OK;
}

enum SPUState VerifySPUFunc(struct SPU *spu)
{
    if(spu == NULL)
    {
        return SPU_BAD_MAIN_POINTER;
    }

    if(spu->ip < 0)
    {
        return SPU_BAD_IP;
    }
    if(spu->code == NULL)
    {
        return SPU_BAD_CODE;
    }
    if(spu->code_size < 0)
    {
        return SPU_BAD_CODE_SIZE;
    }

    if(spu->code[1] != COMMANDS_VERSION)
    {
        return SPU_VERSION_NOT_OK;
    }//TODO: VERIFY SPU

    return SPU_OK;
}

const char* SPUGetErrName(enum SPUState err)
{
    switch(err)
    {
        case SPU_OK:
            return "SPU_OK";
            break;

        case SPU_ERROR:
            return "SPU_ERROR";
            break;

        case SPU_DELETED:
            return "SPU_DELETED";
            break;

        case SPU_VERSION_NOT_OK:
            return "SPU_VERSION_NOT_OK";
            break;

        case SPU_BAD_MAIN_POINTER:
            return "SPU_BAD_MAIN_POINTER";
            break;

        case SPU_BAD_STACK:
            return "SPU_BAD_STACK";
            break;

        case SPU_BAD_IP:
            return "SPU_BAD_IP";
            break;

        case SPU_BAD_CODE:
            return "SPU_BAD_CODE";
            break;

        case SPU_BAD_CODE_SIZE:
            return "SPU_BAD_CODE_SIZE";
            break;

        default:
            return "UNDEFINED_ERROR";
            break;
    }
}

int SPUDumpFunc(  struct SPU *spu,
                    const char *spu_name,
                    const int line,
                    const char *file_name,
                    const char *func_name)
{
    assert(spu_name);
    assert(file_name);
    assert(func_name);

    enum SPUState err = VerifySPUFunc(spu);

    printf("=====SPU DUMP=====\n");

    const char *err_name = SPUGetErrName(err);
    printf("SPU error (%d) - %s\n", err, err_name);

    printf("Location: %s:%d in function %s\n", file_name, line, func_name);
    printf("SPU name: %s | SPU address: %p\n", spu_name, spu);
    printf("SPU Stack: ");
    PrintStack(spu->stk);
    for (int i = 0; i < spu->code_size; i++)
    {
        printf("%02X ", spu->code[i]);
    }
    printf("\n");
    for (int i = 0; i < spu->ip * 3; i++)
    {
        printf(" ");
    }
    printf("^^\n");

    return 1;
}

enum SPUState ExecuteCode(struct SPU *spu)
{
    int a = 0;
    int b = 0;
    int val = 0;
    int reg_num = 0;
    int command = -1;

    while(spu->ip <= spu->code_size && command != CMD_HLT)
    {
        VerifySPU(spu);

        command = spu->code[spu->ip];

        switch(command)
        {
            case CMD_ADD:
                StackPop(spu->stk, &a);
                StackPop(spu->stk, &b);
                spu->ip++;
                StackPush(spu->stk, a + b);
                break;

            case CMD_PUSH:
                val = spu->code[spu->ip + 1];
                StackPush(spu->stk, val);
                spu->ip += 2;
                break;

            case CMD_DIV:
                StackPop(spu->stk, &a);
                StackPop(spu->stk, &b);
                spu->ip++;
                StackPush(spu->stk, b/a);
                break;

            case CMD_OUT:
                StackPop(spu->stk, &val);
                printf("%d\n", val);
                spu->ip++;
                break;

            case CMD_PUSHREG:
                reg_num = spu->code[spu->ip + 1];
                StackPush(spu->stk, spu->regs[reg_num]);
                spu->ip += 2;
                break;

            case CMD_POPREG:
                reg_num = spu->code[spu->ip + 1];
                StackPop(spu->stk, &(spu->regs[reg_num]));
                spu->ip += 2;
                break;

            case CMD_MUL:
                StackPop(spu->stk, &a);
                StackPop(spu->stk, &b);
                spu->ip++;
                StackPush(spu->stk, a*b);
                break;

            case CMD_SUB:
                StackPop(spu->stk, &a);
                StackPop(spu->stk, &b);
                spu->ip++;
                StackPush(spu->stk, b - a);
                break;

            case CMD_JB:
                StackPop(spu->stk, &b);
                StackPop(spu->stk, &a);
                if(a < b)
                {
                    spu->ip = spu->code[spu->ip + 1] + HEADER_SIZE;
                }

                else
                {
                    spu->ip += 2;
                }

                break;

            case CMD_JBE:
                StackPop(spu->stk, &b);
                StackPop(spu->stk, &a);

                if(a <= b)
                {
                    spu->ip = spu->code[spu->ip + 1] + HEADER_SIZE;
                }

                else
                {
                    spu->ip += 2;
                }

                break;

            case CMD_JA:
                StackPop(spu->stk, &b);
                StackPop(spu->stk, &a);

                if(a > b)
                {
                    spu->ip = spu->code[spu->ip + 1] + HEADER_SIZE;
                }

                else
                {
                    spu->ip += 2;
                }

                break;

            case CMD_JAE:
                StackPop(spu->stk, &b);
                StackPop(spu->stk, &a);

                if(a >= b)
                {
                    spu->ip = spu->code[spu->ip + 1] + HEADER_SIZE;
                }

                else
                {
                    spu->ip += 2;
                }

                break;

            case CMD_JE:
                StackPop(spu->stk, &b);
                StackPop(spu->stk, &a);

                if(a == b)
                {
                    spu->ip = spu->code[spu->ip + 1] + HEADER_SIZE;
                }

                else
                {
                    spu->ip += 2;
                }

                break;

            case CMD_JNE:
                StackPop(spu->stk, &b);
                StackPop(spu->stk, &a);

                if(a != b)
                {
                    spu->ip = spu->code[spu->ip + 1] + HEADER_SIZE;
                }

                else
                {
                    spu->ip += 2;
                }

                break;

            case CMD_HLT:
                break;

            default:
                break;
        }
        VerifySPU(spu);
    }

    VerifySPU(spu);

    return SPU_OK;
}
