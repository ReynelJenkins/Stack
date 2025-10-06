#ifndef STACK_H
#define STACK_H

typedef int stack_element;

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <assert.h>

typedef struct Stack Stack_t;

const stack_element CANARY = (int)0xDEADC0DE;

extern uint64_t HASH_KEY;
extern bool HASH_PROTECTION;

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

enum StackErrors StackCtor(Stack_t **stk, ssize_t cap);
enum StackErrors ExpandStack (Stack_t *stk, ssize_t new_cap);
void DeleteStack(Stack_t **stk);
enum StackErrors StackPush(Stack_t *stk, stack_element val);
enum StackErrors StackPop(Stack_t *stk, stack_element *val);
enum StackErrors VerifyStackFunc(Stack_t *stk);
int StackDumpFunc(  Stack_t *stk,
                    enum StackErrors err,
                    const char *var_name,
                    const int line,
                    const char *file_name,
                    const char *func_name);
void PrintStack(Stack_t *stk);
const char* StackGetErrName(enum StackErrors err);
void InitSecurity();
uint64_t StackHash(Stack_t *stk);

#define StackDump(s, err) StackDumpFunc(s, err, #s, __LINE__, __FILE__, __FUNCTION__)

#define VerifyStack(stk) do {                       \
    enum StackErrors error = VerifyStackFunc(stk);  \
    if(error)                                       \
    {                                               \
        StackDump(stk, error);                      \
        return error;                               \
    }                                               \
} while(0)

#endif //STACK_H
