#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "stack_funcs_hash.h"
#include "error_func.h"

#define LINE __LINE__
#define FILE __FILE__

#define STACK_DUMP(myStack) StackDump(myStack, stackErrors,  FILE, LINE, __func__);

#define STACK_VERIFY(myStack, stackErrors) { if (StackVerify(myStack, stackErrors) > 0) { \
        PrintStackErrors(stackErrors); \
        STACK_DUMP(myStack); }\
}

int StackFuncHash ()
{
    Stack myStack = {
        0,
        NULL,
        0,
        0,
        0,
        0
    };

    struct StackErrors stackErrors = {
        (1 << 0), // ERROR_SIZE_BIT
        (1 << 1), // ERROR_CAPACITY_BIT
        (1 << 2), // ERROR_DATA_BIT
        (1 << 3), // ERROR_PUSH_BIT
        (1 << 4), // ERROR_HASH_BIT
        (1 << 5), //ERROR_CANARY_START_BIT;
        (1 << 6), //ERROR_CANARY_END_BIT;
    };


    StackCtor(&myStack, &stackErrors);

    for (int i = 0; i <= 20; i++) {
        StackPush(&myStack, i * 10, &stackErrors);
    }


    int size = myStack.size;
    for (int i = 0; i < size; i++) {
        fprintf(LOG_FILE, "%d\n", StackPop(&myStack, &stackErrors));
    }

    StackDtor (&myStack, &stackErrors);

    return 0;
}

void StackRellocUp(struct Stack *myStack, float koef_capacity, struct StackErrors* stackErrors)
{
    fprintf(LOG_FILE, "я - StackRellocUp1\n");
    STACK_VERIFY(myStack, stackErrors);

    fprintf(LOG_FILE, "я - StackRellocUp\n");
    fprintf(LOG_FILE, "capacity!!!перед изменением = %d\n", myStack->capacity);
    myStack->capacity *= koef_capacity;
    fprintf(LOG_FILE, "capacity!!!после изменений = %d\n", myStack->capacity);
    myStack->data = (Elem_t*)realloc(myStack->data, sizeof(Elem_t) * myStack->capacity + 2 * sizeof(BUF_CANARY));
}


void StackDump(struct Stack* myStack, struct StackErrors* stackErrors, const char *file, int line, const char *function)
{
    fprintf(LOG_FILE, "\nTime is %s\n", __TIME__);
    printf("I'm stackDump called from %s (%d) %s\n", function, line, file);
    fprintf(LOG_FILE, "Stack[%p] \"myStack\" from %s(%d) in function - %s.\n", myStack->data, FILE, LINE, __func__);

    fprintf(LOG_FILE, "myStack->size = %d\n", myStack->size);
    int now_size = myStack->size;
    fprintf(LOG_FILE, "now_size = %d\n", now_size);


    for (int i = 0; i < now_size; i++) {
        fprintf(LOG_FILE, "data[%d] = %d\n", i, myStack->data[i]);
    }

    fprintf(LOG_FILE, "size = %d\n", myStack->size);
    fprintf(LOG_FILE, "capacity = %d\n", myStack->capacity);
    fprintf(LOG_FILE, "data[%p]\n", myStack->data);
}

void StackCtor(struct Stack* myStack, struct StackErrors* stackErrors)
{
    myStack->capacity = Capacity;
    myStack->data = (Elem_t*)calloc(myStack->capacity * sizeof(Elem_t) + 2 * sizeof(BUF_CANARY), sizeof(char));
    *(unsigned int *)myStack->data = BUF_CANARY;
    *(unsigned int*)((char*) myStack->data + sizeof(BUF_CANARY) + sizeof(Elem_t) * myStack->capacity) = BUF_CANARY;

    fprintf (LOG_FILE, "я в сторе\n");

    myStack->size = 0;
    myStack->hash = CalculateHash(myStack); // Рассчитываем и сохраняем хэш-код

    STACK_VERIFY(myStack, stackErrors);

    fprintf(LOG_FILE, "CalculateHash = %u\n", CalculateHash(myStack));
    fprintf(LOG_FILE, "myStack->hash = %u\n", myStack->hash);
}

void StackPush(struct Stack* myStack, Elem_t value, struct StackErrors* stackErrors)
{
   STACK_VERIFY(myStack, stackErrors);

    if (myStack->size >= myStack->capacity) {
        float koef_capacity = 2.0;
        StackRellocUp(myStack, koef_capacity, stackErrors);
    }

    else if (myStack->size < (myStack->capacity) / 2)
    {
        float koef_capacity = 0.5;
        StackRellocUp(myStack, koef_capacity, stackErrors);
    }

    fprintf (LOG_FILE, "я в пуше\n");

    myStack->data[myStack->size++] = value;

    STACK_VERIFY(myStack, stackErrors);
}

void StackDtor(struct Stack* myStack, struct StackErrors* stackErrors)
{
    free(myStack->data);
    myStack->data = NULL;
    myStack->capacity = 0;
    myStack->size = 0;
}

Elem_t StackPop(struct Stack* myStack, struct StackErrors* stackErrors)
{
    STACK_VERIFY(myStack, stackErrors);

    return myStack->data[--myStack->size];
}

unsigned int CalculateHash (struct Stack* myStack)
{
    const char* data = (const char*)myStack->data;
    unsigned int hash = 0;

    hash = (hash * HASH_CONST + sizeof(myStack->capacity));
    hash = (hash * HASH_CONST + sizeof(Elem_t) * myStack->size);

    for (int i = 0; i < myStack->size; i++) {
        hash = (hash * HASH_CONST + data[i]);
    }

    return hash;
}

Elem_t* ReturnData (struct Stack* myStack)
{
    return myStack -> data + sizeof (BUF_CANARY);
}

