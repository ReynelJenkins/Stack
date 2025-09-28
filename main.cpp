#include "stack.h"


int main()
{
    InitSecurity();
    Stack_t *my_stack = nullptr;
    StackInit(&my_stack, 2);
    stack_element var = 123;
    StackPush(my_stack, 111);
    StackPush(my_stack, 222);
    ExpandStack(my_stack, 5);
    StackPush(my_stack, 333);
    StackPop(my_stack, &var);
    printf("%d\n", var);
    //
    StackPop(my_stack, &var);
    printf("%d\n", var);
    StackPop(my_stack, &var);
    printf("%d\n", var);
    StackPush(my_stack, 1111);
    StackPush(my_stack, 111);
    StackPush(my_stack, 222);
    //*((int *)((int)my_stack + 11)) = 123;
    StackPop(my_stack, &var);
    //DeleteStack(&my_stack);
    StackPush(my_stack, 444);
    StackPush(my_stack, 444);
    StackPush(my_stack, 444);
    return 0;
}
