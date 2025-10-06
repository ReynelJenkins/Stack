#ifndef SOFTPROCESSOR_H
#define SOFTPROCESSOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

const int START_CODE_SIZE = 8;
const int MAX_COMMAND_SIZE = 256;
const int COMMANDS_VERSION = 2;
const char *BYTE_CODE_SOURCE_FILE = "byte_code.bin";
const int START_STACK_SIZE = 8;

enum CMD
{
    CMD_ADD     = 1,
    CMD_PUSH    = 2,
    CMD_DIV     = 3,
    CMD_OUT     = 4,
    CMD_HLT     = 5,
    CMD_PUSHREG = 6,
    CMD_POPREG  = 7,
    CMD_MUL     = 8,
    CMD_SUB     = 9
};

#endif //SOFTPROCESSOR_H
