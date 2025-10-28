#ifndef SOFTPROCESSOR_H
#define SOFTPROCESSOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

const int START_CODE_SIZE = 8;
const int MAX_COMMAND_SIZE = 256;
const int COMMANDS_VERSION = 3;
const char *BYTE_CODE_SOURCE_FILE = "byte_code.bin";
const int SIGN = 0xDEADC0DE;
const int HEADER_SIZE = 3;
const int START_STACK_SIZE = 8;

enum CMD
{
    CMD_ADD     = 0,
    CMD_MUL     = 1,
    CMD_SUB     = 2,
    CMD_DIV     = 3,
    CMD_SQRT    = 4,

    CMD_IN      = 5,
    CMD_OUT     = 6,

    CMD_PUSH    = 7,

    CMD_PUSHREG = 8,
    CMD_POPREG  = 9,

    CMD_PUSHM   = 10,
    CMD_POPM    = 11,

    CMD_JB      = 12,
    CMD_JBE     = 13,
    CMD_JA      = 14,
    CMD_JAE     = 15,
    CMD_JE      = 16,
    CMD_JNE     = 17,
    CMD_JMP     = 18,

    CMD_CALL    = 19,
    CMD_RET     = 20,
    CMD_HLT     = 21
};

#endif //SOFTPROCESSOR_H
