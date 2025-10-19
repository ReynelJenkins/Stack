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
const int SIGN = 0xDA;
const int HEADER_SIZE = 3;
const int START_STACK_SIZE = 8;

enum CMD
{
    CMD_HLT     = 0,

    CMD_ADD     = 1,
    CMD_MUL     = 2,
    CMD_SUB     = 3,
    CMD_DIV     = 4,

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

    CMD_RET     = 19,
    CMD_CALL    = 20
};

#endif //SOFTPROCESSOR_H
