#include "SoftProcessor.h"
#include "stack.h"
#include "processor.h"
#include "file.h"

int main()
{
    SPU my_spu;
    SPUCtor(&my_spu, START_STACK_SIZE);

    FILE *file = my_fopen(BYTE_CODE_SOURCE_FILE, "rb");
    LoadCodeFromFile(file, &my_spu);
    fclose(file);

    ExecuteCode(&my_spu);

    SPUDtor(&my_spu);

    return 0;
}

enum SPUState SPUCtor(struct SPU *spu, int start_stack_size)
{
    StackCtor(&(spu->stk), start_stack_size);
    StackCtor(&(spu->ret_stk), start_stack_size);

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
    }

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

int SPUDumpFunc(struct SPU *spu,
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
        if (i == spu->ip)
        {
            printf("\033[1;31m%02X \033[0m", spu->code[i]);

            continue;
        }
        printf("%02X ", spu->code[i]);
    }
    printf("\n");
    printf("Registers:\n");
    for (int i = 0; i < 8; i++)
    {
        printf("Register %d: %d\n", i, spu->regs[i]);
    }
    return 1;
}

enum SPUState ExecuteCode(struct SPU *spu)
{
    int command = -1;

    while(spu->ip <= spu->code_size && command != CMD_HLT)
    {
        VerifySPU(spu);
        command = spu->code[spu->ip];

        if (command == CMD_HLT)
        {
            break;
        }

        int found = 0;
        for (int i = 0; i < command_handlers_count; i++)
        {
            if (command_handlers[i].command_code == command)
            {
                command_handlers[i].handler(spu);
                found = 1;

                break;
            }
        }

        if (!found)
        {
            printf("Unknown command code: %d at IP: %d\n", command, spu->ip);
        }

        VerifySPU(spu);
    }

    VerifySPU(spu);

    return SPU_OK;
}

void handle_ADD(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &a);
    StackPop(spu->stk, &b);
    StackPush(spu->stk, a + b);
    spu->ip++;
}

void handle_PUSH(SPU* spu)
{
    int val = spu->code[spu->ip + 1];
    StackPush(spu->stk, val);
    spu->ip += 2;
}

void handle_DIV(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &a);
    StackPop(spu->stk, &b);
    StackPush(spu->stk, b / a);
    spu->ip++;
}

void handle_SQRT(SPU* spu)
{
    int a = 0;
    StackPop(spu->stk, &a);
    StackPush(spu->stk, (int)pow(a, 0.5));
    spu->ip++;
}

void handle_IN(SPU* spu)
{
    int val = 0;
    scanf("%d", &val);
    StackPush(spu->stk, val);
    spu->ip++;
}

void handle_OUT(SPU* spu)
{
    int val = 0;
    StackPop(spu->stk, &val);
    printf("%d\n", val);
    spu->ip++;
}

void handle_PUSHREG(SPU* spu)
{
    int reg_num = spu->code[spu->ip + 1];
    StackPush(spu->stk, spu->regs[reg_num]);
    spu->ip += 2;
}

void handle_POPREG(SPU* spu)
{
    int reg_num = spu->code[spu->ip + 1];
    StackPop(spu->stk, &(spu->regs[reg_num]));
    spu->ip += 2;
}

void handle_PUSHM(SPU* spu)
{
    int addr = spu->regs[spu->code[spu->ip + 1]];
    StackPush(spu->stk, spu->ram[addr]);
    spu->ip += 2;
}

void handle_POPM(SPU* spu)
{
    int addr = spu->regs[spu->code[spu->ip + 1]];
    StackPop(spu->stk, &(spu->ram[addr]));
    spu->ip += 2;
}

void handle_MUL(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &a);
    StackPop(spu->stk, &b);
    StackPush(spu->stk, a * b);
    spu->ip++;
}

void handle_SUB(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &a);
    StackPop(spu->stk, &b);
    StackPush(spu->stk, b - a);
    spu->ip++;
}

void handle_conditional_jump(SPU* spu, bool condition)
{
    if (condition) {
        spu->ip = spu->code[spu->ip + 1] + HEADER_SIZE;
    } else {
        spu->ip += 2;
    }
}

void handle_JB(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &b);
    StackPop(spu->stk, &a);
    handle_conditional_jump(spu, a < b);
}

void handle_JBE(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &b);
    StackPop(spu->stk, &a);
    handle_conditional_jump(spu, a <= b);
}

void handle_JA(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &b);
    StackPop(spu->stk, &a);
    handle_conditional_jump(spu, a > b);
}

void handle_JAE(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &b);
    StackPop(spu->stk, &a);
    handle_conditional_jump(spu, a >= b);
}

void handle_JE(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &b);
    StackPop(spu->stk, &a);
    handle_conditional_jump(spu, a == b);
}

void handle_JNE(SPU* spu)
{
    int a = 0, b = 0;
    StackPop(spu->stk, &b);
    StackPop(spu->stk, &a);
    handle_conditional_jump(spu, a != b);
}

void handle_JMP(SPU* spu)
{
    spu->ip = spu->code[spu->ip + 1] + HEADER_SIZE;
}

void handle_CALL(SPU* spu)
{
    int target_addr = spu->code[spu->ip + 1] + HEADER_SIZE;

    int return_addr = spu->ip + 2;
    StackPush(spu->ret_stk, return_addr);

    spu->ip = target_addr;
}

void handle_RET(SPU* spu)
{
    int return_addr = 0;
    StackPop(spu->ret_stk, &return_addr);

    spu->ip = return_addr;
}
