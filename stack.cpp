#include "stack.h"

uint64_t HASH_KEY = 0; // Ключ для хэш функции
bool HASH_PROTECTION = false;

struct Stack
{
    stack_element canary1;

    ssize_t capacity;
    ssize_t size;
    uint64_t hash;
    stack_element *data;

    stack_element canary2;
};


//TODO: упорядочить функции по логическому смыслу
enum StackErrors StackCtor (Stack_t **stk, ssize_t cap)
{
    if (cap <= 0)
    {
        return BAD_STACK_CAPACITY;
    }

    *stk = (Stack_t *)calloc(1, sizeof(Stack_t));
    if(*stk == NULL)
    {
        return BAD_STACK_MAIN_POINTER;
    }

    (*stk)->canary1 = CANARY;
    (*stk)->canary2 = CANARY;

    (*stk)->data = (stack_element*)calloc(cap + 2, sizeof(stack_element));
    if((*stk)->data == NULL)
    {
        free(*stk);
        return BAD_STACK_DATA;
    }

    (*stk)->capacity = cap + 2; // + 2 для двух canary
    (*stk)->size = 1;

    ((*stk)->data)[0] = CANARY;
    ((*stk)->data)[(*stk)->capacity - 1] = CANARY;

    if (HASH_PROTECTION)
    {
        (*stk)->hash = StackHash(*stk);
    }

    return STACK_OK;
}

enum StackErrors ExpandStack (Stack_t *stk, ssize_t new_cap)
{
    VerifyStack(stk);

    if (new_cap <= 0)
    {
        return BAD_STACK_CAPACITY;
    }

    stk->data = (stack_element*)realloc(stk->data, (new_cap + 2) * sizeof(stack_element));
    if(stk->data == NULL)
    {
        return BAD_STACK_DATA;
    }

    stk->capacity = new_cap + 2;
    (stk->data)[stk->capacity - 1] = CANARY;

    if (HASH_PROTECTION)
    {
        stk->hash = StackHash(stk);
    }

    VerifyStack(stk);

    return STACK_OK;
}

void DeleteStack(Stack_t **stk)
{
    free((*stk)->data);
    free(*stk);
    *stk = nullptr;
}

enum StackErrors StackPush(Stack_t *stk, stack_element val) //TODO: Переделать структуры функций StackPush StackPop
{
    //TODO: Убрать ассерты проверки стека тк в рилизе они отключатся
    VerifyStack(stk);

    if (stk->size < 1 || stk->size > stk->capacity - 1)
    {
        return BAD_STACK_SIZE;
    }

    if (stk->size == stk->capacity - 1)
    {
        //TODO: расширение стека если не хватаем места
        ExpandStack(stk, (stk->capacity - 2) * 2);

        VerifyStack(stk);
    }

    stk->data[stk->size] = val;
    stk->size += 1;

    if (HASH_PROTECTION)
    {
        stk->hash = StackHash(stk);
    }

    VerifyStack(stk);

    return STACK_OK;
}

enum StackErrors StackPop(Stack_t *stk, stack_element *val)
{
    assert(val); //можно ли оставить ассерт на val?

    VerifyStack(stk);

    if (stk->size > stk->capacity - 1 || stk->size <= 1)
    {
        return BAD_STACK_SIZE;
    }

    stk->size -= 1;
    *val = stk->data[stk->size];

    if (HASH_PROTECTION)
    {
        stk->hash = StackHash(stk);
    }

    VerifyStack(stk);

    return STACK_OK;
}

enum StackErrors VerifyStackFunc(Stack_t *stk)
{
    if(!stk)
    {
        return BAD_STACK_MAIN_POINTER;
    }

    if(stk->canary1 != CANARY || stk->canary2 != CANARY)
    {
        return BAD_STACK_CORRUPTED_DATA;
    }

    if(stk->capacity <= 0)
    {
        return BAD_STACK_CAPACITY;
    }

    if(!(stk->data))
    {
        return BAD_STACK_DATA;
    }

    if (stk->size < 0)
    {
        return BAD_STACK_SIZE;
    }

    if (HASH_PROTECTION == true && stk->hash != StackHash(stk))
    {
        return BAD_STACK_CORRUPTED_DATA;
    }

    if((stk->data)[0] != CANARY || (stk->data)[stk->capacity - 1] != CANARY)
    {
        return BAD_STACK_CORRUPTED_DATA;
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
    assert(var_name);
    assert(file_name);
    assert(func_name);

    printf("=====STACK DUMP=====\n");

    const char *err_name = StackGetErrName(err);
    printf("Stack error (%d) - %s\n", err, err_name);

    printf("Location: %s:%d in function %s\n", file_name, line, func_name);
    printf("Stack name: %s | Stack address: %p\n", var_name, stk);
    return 1;
}

void PrintStack(Stack_t *stk)
{
    for(int i = 1; i < stk->size; i++)
    {
        printf("%d ", stk->data[i]);
    }

    printf("\n");
}

const char* StackGetErrName(enum StackErrors err)
{
    //TODO: Сравнивать со значениями enum
    switch(err)
    {
        case STACK_OK:
            return "STACK_OK";
            break;

        case BAD_STACK_MAIN_POINTER:
            return "BAD_STACK_MAIN_POINTER";
            break;

        case BAD_STACK_SIZE:
            return "BAD_STACK_SIZE";
            break;

        case BAD_STACK_CAPACITY:
            return "BAD_STACK_CAPACITY";
            break;

        case BAD_STACK_DATA:
            return "BAD_STACK_DATA";
            break;

        case BAD_STACK_CORRUPTED_DATA:
            return "BAD_STACK_CORRUPTED_DATA";
            break;

        case BAD_STACK:
            return "BAD_STACK";
            break;

        default:
            return "UNDEFINED_ERROR";
            break;
    }
}

void InitSecurity()
{
    srand(time(NULL));
    HASH_KEY = ((uint64_t)rand() << 32) ^ rand();
    HASH_PROTECTION = true;
}

uint64_t StackHash(Stack *stk)
{
    uint64_t h = HASH_KEY;
    h ^= stk->size;
    h *= 1099511628211ULL;

    h ^= stk->capacity;
    h *= 1099511628211ULL;

    for (ssize_t i = 0; i < stk->capacity + 2; i++) {
        h ^= (uint64_t)stk->data[i];
        h *= 1099511628211ULL;
    }
    return h;
}
