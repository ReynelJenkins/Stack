#include "stack.h"

int main()
{
    InitSecurity();
    struct Stack my_stack = {};
    StackInit(&my_stack, 3);
    free(my_stack.data); // segmentation fault
    stack_element var = 123;
    StackPush(&my_stack, 111);
    StackPush(&my_stack, 222);
    StackPush(&my_stack, 333);
    StackPop(&my_stack, &var);
    printf("%d\n", var);
    StackPop(&my_stack, &var);
    printf("%d\n", var);
    StackPop(&my_stack, &var);
    printf("%d\n", var);
    StackPush(&my_stack, 1111);
    StackPush(&my_stack, 111);
    free(my_stack.data);
    StackPush(&my_stack, 222);
    return 0;
}
