#include <stdlib.h>

#include "log_funcs.h"
#include "stack.h"

int main()
{
    OpenLogFile ("LOGE", "a");

    Stack myStack = {};

    StackCtor(&myStack);
    STACK_DUMP(&myStack);

    unsigned now_size = SIZE;
    for (int i = 1; i <= now_size; i++) {
        StackPush(&myStack, i * 10);
    }

    //myStack.data[3] += 107;

    STACK_DUMP(&myStack);

    fprintf (LOG_FILE, "POPING\n");
    int size =  myStack.size;
    for (int i = 0; i < size; i++) {
        fprintf(LOG_FILE, "%lf\n", StackPop(&myStack));
    }

    //myStack.canary_start = 0;

    STACK_DUMP(&myStack);

    StackDtor(&myStack);

    return 0;
}

//-DWITH_CANARY_AND_HASHE

