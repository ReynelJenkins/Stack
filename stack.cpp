#include "stack.h"

uint64_t SECRET_KEY = 0;

struct Stack
{
    ssize_t capacity = 0;
    ssize_t size = 0;
    uint64_t hash = 0;

    stack_element *data = nullptr;
};


enum StackErrors StackInit (Stack_t **stk, size_t cap)
{
    *stk = (Stack_t *)calloc(1, sizeof(Stack_t));
    assert(*stk);
    (*stk)->capacity = cap;
    (*stk)->size = 0;
    (*stk)->data = (int*)calloc(cap, sizeof(stack_element));
    assert((*stk)->data);
    (*stk)->hash = StackHash(*stk);

    return STACK_OK;
}

enum StackErrors StackPush(Stack_t *stk, stack_element val)
{
    enum StackErrors error = CheckStack(stk, STACK_OPERATIONS_PUSH);
    if(!error)
    {
        stk->data[stk->size] = val;
        stk->size += 1;
        stk->hash = StackHash(stk);

        return STACK_OK;
    }

    else
    {
        StackDump(stk, error);
        return error;
    }
}

enum StackErrors StackPop(Stack_t *stk, stack_element *val)
{
    enum StackErrors error = CheckStack(stk, STACK_OPERATIONS_POP);
    if(!error)
    {
        stk->size -= 1;
        *val = stk->data[stk->size];
        stk->hash = StackHash(stk);

        return STACK_OK;
    }

    else
    {
        StackDump(stk, error);
        return error;
    }
}

enum StackErrors CheckStack(Stack_t *stk, enum StackOperations op)
{
    if(!stk)
    {
        return BAD_STACK_MAIN_POINTER;
    }

    if(stk->capacity <= 0)
    {
        return BAD_STACK_CAPACITY;
    }

    if(!(stk->data))
    {
        return BAD_STACK_DATA;
    }

    if (stk->hash != StackHash(stk))
    {
        return BAD_STACK_CORRUPTED_DATA;
    }

    switch(op)
    {
        case STACK_OPERATIONS_PUSH:
            if (stk->size >= stk->capacity || stk->size < 0)
            {
                return BAD_STACK_SIZE;
            }
            break;

        case STACK_OPERATIONS_POP:
            if (stk->size > stk->capacity || stk->size <= 0)
            {
                return BAD_STACK_SIZE;
            }
            break;

        default:
            return BAD_STACK;
            break;
    }

    return STACK_OK;
}

int StackDumpFunc(  Stack_t *stk,
                    enum StackErrors err,
                    const char *var_name,
                    const int line,
                    const char *file_name,
                    const char *func_name)
{
    printf("=====STACK DUMP=====\n");

    const char *err_name = GetErrName(err);
    printf("Stack error (%d) - %s\n", err, err_name);

    printf("Location: %s:%d in function %s\n", file_name, line, func_name);
    printf("Stack name: %s | Stack address: %p\n", var_name, stk);
    return 1;
}

const char* GetErrName(enum StackErrors err)
{
    switch(err)
    {
    case 0:
        return "STACK_OK";
        break;

    case 1:
        return "BAD_STACK_MAIN_POINTER";
        break;

    case 2:
        return "BAD_STACK_SIZE";
        break;

    case 3:
        return "BAD_STACK_CAPACITY";
        break;

    case 4:
        return "BAD_STACK_DATA";
        break;

    case 5:
        return "BAD_STACK_CORRUPTED_DATA";
        break;

    case 6:
        return "BAD_STACK";
        break;

    default:
        return "UNDEFINED_ERROR";
        break;
    }
}

void InitSecurity()
{
    srand((unsigned)time(NULL) ^ (uintptr_t)&SECRET_KEY);
    SECRET_KEY = ((uint64_t)rand() << 32) ^ rand();
}

uint64_t StackHash(Stack *st)
{
    uint64_t h = SECRET_KEY;
    h ^= st->size;
    h *= 1099511628211ULL;

    h ^= st->capacity;
    h *= 1099511628211ULL;

    for (ssize_t i = 0; i < st->size; i++) {
        h ^= (uint64_t)st->data[i];
        h *= 1099511628211ULL;
    }
    return h;
}
