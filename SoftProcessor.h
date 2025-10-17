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
    CMD_PUSH    = 2,
    CMD_DIV     = 3,
    CMD_OUT     = 4,
    CMD_PUSHREG = 5,
    CMD_POPREG  = 6,
    CMD_MUL     = 7,
    CMD_SUB     = 8,
    CMD_JB      = 9,
    CMD_JBE     = 10,
    CMD_JA      = 11,
    CMD_JAE     = 12,
    CMD_JE      = 13,
    CMD_JNE     = 14,
    CMD_JMP     = 15,
};

#endif //SOFTPROCESSOR_H
