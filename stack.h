#ifndef STACK_H
#define STACK_H

typedef int stack_element;

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>

extern uint64_t SECRET_KEY;

typedef struct Stack Stack_t;

enum StackErrors
{
    STACK_OK                 = 0,
    BAD_STACK_MAIN_POINTER   = 1,
    BAD_STACK_SIZE           = 2,
    BAD_STACK_CAPACITY       = 3,
    BAD_STACK_DATA           = 4,
    BAD_STACK_CORRUPTED_DATA = 5,
    BAD_STACK                = 6
};

enum StackOperations
{
    STACK_OPERATIONS_PUSH = 0,
    STACK_OPERATIONS_POP  = 1
};

enum StackErrors StackInit(Stack_t *stk, size_t cap);
enum StackErrors StackPush(Stack_t *stk, stack_element val);
enum StackErrors StackPop(Stack_t *stk, stack_element *val);
enum StackErrors CheckStack(Stack_t *stk, enum StackOperations op);
int StackDumpFunc(  Stack_t *stk,
                    enum StackErrors err,
                    const char *var_name,
                    const int line,
                    const char *file_name,
                    const char *func_name);
const char* GetErrName(enum StackErrors err);
void InitSecurity();
uint64_t StackHash(Stack_t *st);

#define StackDump(s, err) StackDumpFunc(s, err, #s, __LINE__, __FILE__, __FUNCTION__)

#endif //STACK_H
