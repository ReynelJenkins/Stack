#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define MAX_LABELS 64

typedef struct {
    char name[32];
    int addr;
} Label;

Label labels[MAX_LABELS];
int label_count = 0;
int second_run_required = 0;
int pass = 1;

enum ArgType
{
    ARG_TYPE_NO_ARG        = 0,
    ARG_TYPE_VALUE         = 1,
    ARG_TYPE_REGISTER      = 2,
    ARG_TYPE_LABEL_NUMBER  = 3,
    ARG_TYPE_ADDR_REGISTER = 4
};

struct Command
{
    const char* name;
    int command_code;
    enum ArgType has_arg;
    const char* arg_type;
};

struct AssemblerValues
{
    int *code = nullptr;
    int size = START_CODE_SIZE;
    int pos = 0;
    int line = 0;
    int val = 0;
    char reg_name[8];
    char cmdstr[MAX_COMMAND_SIZE] = {};
};


Command commands[] = {
    {"ADD",     CMD_ADD,     ARG_TYPE_NO_ARG,        NULL},
    {"SUB",     CMD_SUB,     ARG_TYPE_NO_ARG,        NULL},
    {"MUL",     CMD_MUL,     ARG_TYPE_NO_ARG,        NULL},
    {"DIV",     CMD_DIV,     ARG_TYPE_NO_ARG,        NULL},
    {"OUT",     CMD_OUT,     ARG_TYPE_NO_ARG,        NULL},
    {"HLT",     CMD_HLT,     ARG_TYPE_NO_ARG,        NULL},
    {"IN",      CMD_IN,      ARG_TYPE_NO_ARG,        NULL},
    {"RET",     CMD_RET,     ARG_TYPE_NO_ARG,        NULL},
    {"SQRT",    CMD_SQRT,    ARG_TYPE_NO_ARG,        NULL},
    {"PUSH",    CMD_PUSH,    ARG_TYPE_VALUE,         "value"},
    {"PUSHREG", CMD_PUSHREG, ARG_TYPE_REGISTER,      "register"},
    {"POPREG",  CMD_POPREG,  ARG_TYPE_REGISTER,      "register"},
    {"PUSHM",   CMD_PUSHM,   ARG_TYPE_ADDR_REGISTER, "register"},
    {"POPM",    CMD_POPM,    ARG_TYPE_ADDR_REGISTER, "register"},
    {"JB",      CMD_JB,      ARG_TYPE_LABEL_NUMBER,  "label/number"},
    {"JBE",     CMD_JBE,     ARG_TYPE_LABEL_NUMBER,  "label/number"},
    {"JA",      CMD_JA,      ARG_TYPE_LABEL_NUMBER,  "label/number"},
    {"JAE",     CMD_JAE,     ARG_TYPE_LABEL_NUMBER,  "label/number"},
    {"JE",      CMD_JE,      ARG_TYPE_LABEL_NUMBER,  "label/number"},
    {"JNE",     CMD_JNE,     ARG_TYPE_LABEL_NUMBER,  "label/number"},
    {"JMP",     CMD_JMP,     ARG_TYPE_LABEL_NUMBER,  "label/number"},
    {"CALL",    CMD_CALL,    ARG_TYPE_LABEL_NUMBER,  "label/number"},
};

const int commands_count = sizeof(commands) / sizeof(commands[0]);


size_t GetFileSize(const char *file_name);
int *TranslateToByteCode(FILE *f, int *code_size);
int WriteByteCode(FILE *f, int *code, int size);
int GetRegNumber(const char *reg_name);
int GetLabelPos(const char *label);
int GetArgument(struct AssemblerValues *Vals, struct Command command);
int GetCommand(char *cmd, struct AssemblerValues *asm_vals);
int ExpandCodeBuffer(struct AssemblerValues *asm_vals);
int FindLabel(char *cmd, struct AssemblerValues *asm_vals);
void FindComments(char *cmdstr);

#endif //ASSEMBLER_H
