#include <stdlib.h>

#include "log_funcs.h"
#include "stack.h"

int main()
{
    OpenLogFile ("LOGE", "w");

    Stack myStack = {};

    struct StackErrors stackErrors = {};

    StackCtor(&myStack, &stackErrors);
    STACK_DUMP(&myStack, &stackErrors);

    for (int i = 1; i <= SIZE; i++) {
        StackPush(&myStack, i * 10, &stackErrors);
    }

    myStack.data[myStack.capacity - 1] = 107;
    myStack.data[myStack.capacity - 3] = 107;

    STACK_DUMP(&myStack, &stackErrors);

    fprintf (LOG_FILE, "POPING\n");
    int size =  myStack.size;
    for (int i = 0; i < size; i++) {
        fprintf(LOG_FILE, "%lf\n", StackPop(&myStack, &stackErrors));
    }

    myStack.canary_start = 0;

    STACK_DUMP(&myStack, &stackErrors);

    StackDtor(&myStack, &stackErrors);

    return 0;
}

//-DWITH_CANARY_AND_HASHE

