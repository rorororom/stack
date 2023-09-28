#include <stdlib.h>

#include "log_funcs.h"
#include "stack_funcs_hash.h"

int main()
{
    OpenLogFile ("LOGE", "w");

    Stack myStack = {
#ifdef DWITH_CANARY_AND_HASHE
        0,
#endif
        NULL,
        0,
        0,
#ifdef DWITH_CANARY_AND_HASHE
        0,
        0
#endif
    };

    struct StackErrors stackErrors = {};

    StackCtor(&myStack, &stackErrors);
    STACK_DUMP(&myStack, &stackErrors);

#ifdef WITH_CANARY_AND_HASHE
    myStack.data[++(myStack.size)];
#endif

    for (int i = 1; i <= SIZE; i++) {
        StackPush(&myStack, i * 10, &stackErrors);
    }

    STACK_DUMP(&myStack, &stackErrors);

    fprintf (LOG_FILE, "попоюсь\n");
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

