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
    CMD_HLT     = 0,

    CMD_ADD     = 1,
    CMD_MUL     = 2,
    CMD_SUB     = 3,
    CMD_DIV     = 4,
    CMD_SQRT    = 5,

    CMD_IN      = 6,
    CMD_OUT     = 7,

    CMD_PUSH    = 8,

    CMD_PUSHREG = 9,
    CMD_POPREG  = 10,

    CMD_PUSHM   = 11,
    CMD_POPM    = 12,

    CMD_JB      = 13,
    CMD_JBE     = 14,
    CMD_JA      = 15,
    CMD_JAE     = 16,
    CMD_JE      = 17,
    CMD_JNE     = 18,
    CMD_JMP     = 19,

    CMD_RET     = 20,
    CMD_CALL    = 21
};

#endif //SOFTPROCESSOR_H
