#ifndef PROCESSOR_H
#define PROCESSOR_H

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
    int ip = 1;
    int regs[8] = {};
    Stack_t *stk = nullptr;
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

#endif //PROCESSOR_H
