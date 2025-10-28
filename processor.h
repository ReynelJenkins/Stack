#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <math.h>

#define SPUDump(s) SPUDumpFunc(s, #s, __LINE__, __FILE__, __FUNCTION__)

#define VerifySPU(spu) do {                         \
    enum SPUState error = VerifySPUFunc(spu);       \
    if(error)                                       \
    {                                               \
        SPUDump(spu);                               \
        return error;                               \
    }                                               \
} while(0)

struct SPU
{
    int *code = nullptr;
    int code_size = 0;
    int ip = HEADER_SIZE;
    int regs[8] = {};
    int ram[1024] = {};
    Stack_t *stk = nullptr;
    Stack_t *ret_stk = nullptr;
};

enum SPUState
{
    SPU_OK                  = 0,
    SPU_ERROR               = 1,
    SPU_DELETED             = 2,
    SPU_VERSION_NOT_OK      = 3,
    SPU_BAD_MAIN_POINTER    = 4,
    SPU_BAD_STACK           = 5,
    SPU_BAD_IP              = 6,
    SPU_BAD_CODE            = 7,
    SPU_BAD_CODE_SIZE       = 8
};

enum SPUState LoadCodeFromFile(FILE *f, struct SPU *spu);
enum SPUState SPUCtor(struct SPU *spu, int start_stack_size);
enum SPUState SPUDtor(struct SPU *spu);

enum SPUState VerifySPUFunc(struct SPU *spu);
const char* SPUGetErrName(enum SPUState err);
int SPUDumpFunc(  struct SPU *spu,
                    const char *spu_name,
                    const int line,
                    const char *file_name,
                    const char *func_name);

enum SPUState ExecuteCode(struct SPU *spu);


void handle_ADD(SPU* spu);
void handle_PUSH(SPU* spu);
void handle_DIV(SPU* spu);
void handle_MUL(SPU* spu);
void handle_SUB(SPU* spu);
void handle_SQRT(SPU* spu);

void handle_IN(SPU* spu);
void handle_OUT(SPU* spu);

void handle_PUSHREG(SPU* spu);
void handle_POPREG(SPU* spu);

void handle_PUSHM(SPU* spu);
void handle_POPM(SPU* spu);

void handle_JB(SPU* spu);
void handle_JBE(SPU* spu);
void handle_JA(SPU* spu);
void handle_JAE(SPU* spu);
void handle_JE(SPU* spu);
void handle_JNE(SPU* spu);
void handle_JMP(SPU* spu);

void handle_HLT(SPU* spu);

void handle_conditional_jump(SPU* spu, bool condition);

void handle_CALL(SPU* spu);
void handle_RET(SPU* spu);

typedef void (*CommandHandler)(SPU* spu);

typedef struct {
    int command_code;
    CommandHandler handler;
} CommandExecution;

CommandExecution command_handlers[] = { //ПО порядку
    {CMD_ADD,     handle_ADD},
    {CMD_MUL,     handle_MUL},
    {CMD_SUB,     handle_SUB},
    {CMD_DIV,     handle_DIV},
    {CMD_SQRT,    handle_SQRT},

    {CMD_IN,      handle_IN},
    {CMD_OUT,     handle_OUT},

    {CMD_PUSH,    handle_PUSH},

    {CMD_PUSHREG, handle_PUSHREG},
    {CMD_POPREG,  handle_POPREG},

    {CMD_PUSHM,   handle_PUSHM},
    {CMD_POPM,    handle_POPM},

    {CMD_JB,      handle_JB},
    {CMD_JBE,     handle_JBE},
    {CMD_JA,      handle_JA},
    {CMD_JAE,     handle_JAE},
    {CMD_JE,      handle_JE},
    {CMD_JNE,     handle_JNE},
    {CMD_JMP,     handle_JMP},

    {CMD_CALL,    handle_CALL},
    {CMD_RET,     handle_RET},
};

const int command_handlers_count = sizeof(command_handlers) / sizeof(command_handlers[0]);

#endif //PROCESSOR_H
