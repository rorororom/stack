#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "stack.h"
#include "error_func.h"

void StackDump(struct Stack* myStack, struct StackErrors* stackErrors, const char *file, int line, const char *function)
{
    fprintf(LOG_FILE, "\nTime is %s\n", __TIME__);
    fprintf(LOG_FILE, "I'm stackDump called from %s (%d) %s\n", function, line, file);
    fprintf(LOG_FILE, "Stack[%p] \"myStack\" from %s(%d) in function - %s.\n\n", myStack->data, FILE, LINE, function);

    fprintf(LOG_FILE, "--------------------------------------------------------------------------\n");
#ifdef WITH_CANARY_AND_HASHE
    fprintf(LOG_FILE, "STRUCT CANARY_START = %llu\n\n", myStack -> canary_start);
#endif
    fprintf(LOG_FILE, "Struct:\n");
    fprintf(LOG_FILE, "\tsize = %d\n", myStack->size);
    fprintf(LOG_FILE, "\tcapacity = %d\n", myStack->capacity);
    fprintf(LOG_FILE, "\tадрес data[] = %p\n", myStack->data);

#ifdef WITH_CANARY_AND_HASHE
    fprintf(LOG_FILE, "\nDATA CANARY LEFT  = %llu\n", *PointerLeftCanary(myStack));
#endif

    int now_size = myStack->size;
    for (int i = 0; i < now_size; i++)
    {
        fprintf(LOG_FILE, "\tdata[%d] = %lf\n", i, myStack->data[i]);
    }

#ifdef WITH_CANARY_AND_HASHE
    fprintf(LOG_FILE, "DATA CANARY RIGHT = %llu\n", *PointerRightCanary(myStack));
    fprintf(LOG_FILE, "HASH = %d\n", myStack->hash);
    fprintf(LOG_FILE, "\n");
    fprintf(LOG_FILE, "STRUCT CANARY_END = %llu\n", myStack->canary_end);
#endif
    fprintf(LOG_FILE, "--------------------------------------------------------------------------\n");
}


void StackCtor(struct Stack* myStack, struct StackErrors* stackErrors)
{
    myStack->capacity = Capacity;
    myStack->size = 0;
#ifdef WITH_CANARY_AND_HASHE
    myStack->data = (Elem_t *) calloc(myStack->capacity * sizeof(Elem_t) + 2 * sizeof(Canary_t), sizeof(char));
    myStack->data = (Elem_t *)((Canary_t *)myStack->data + 1);

    *(PointerLeftCanary (myStack)) = BUF_CANARY;
    *(PointerRightCanary (myStack)) = BUF_CANARY;

    myStack->canary_start = BUF_CANARY;
    myStack->canary_end   = BUF_CANARY;

    CALCULATE_HASH(myStack);
#else
    myStack->data = (Elem_t*)calloc(myStack->capacity, sizeof(Elem_t));
#endif
}

void StackPush(struct Stack* myStack, Elem_t value, struct StackErrors* stackErrors)
{
    STACK_VERIFY(myStack, stackErrors);

    if (myStack->size >= myStack->capacity) {
        float koef_capacity = 2.0;
        StackRealloc(myStack, koef_capacity, stackErrors);
    }

    myStack->data[myStack->size++] = value;

#ifdef WITH_CANARY_AND_HASHE
    CALCULATE_HASH(myStack);
#endif

    STACK_VERIFY(myStack, stackErrors);
}

void StackRealloc(struct Stack *myStack, float koef_capacity, struct StackErrors* stackErrors)
{
    STACK_VERIFY(myStack, stackErrors);

    myStack->capacity *= koef_capacity;
#ifdef WITH_CANARY_AND_HASHE
    Elem_t* allocatedData = (Elem_t *)((Canary_t *)myStack->data - 1);
    allocatedData = (Elem_t*)realloc(allocatedData, (myStack->capacity + 2) * sizeof(Elem_t));

    if (allocatedData == NULL)
    {
        printf("ERROR MEMORY FOR REALLOC\n");
    }

    myStack->data = (Elem_t *)((Canary_t *)allocatedData + 1);
#else
    myStack->data = (Elem_t*)realloc(myStack->data, (myStack->capacity + 2) * sizeof(Elem_t));

    if (myStack->data == NULL)
    {
        printf("ERROR MEMORY FOR REALLOC\n");
    }
#endif

#ifdef WITH_CANARY_AND_HASHE
    *(PointerLeftCanary (myStack)) = BUF_CANARY;
    *(PointerRightCanary (myStack)) = BUF_CANARY;
#endif
}

Elem_t StackPop(struct Stack* myStack, struct StackErrors* stackErrors)
{
    STACK_VERIFY(myStack, stackErrors);

    if (myStack->size < (myStack->capacity) / 2)
    {
        float koef_capacity = 0.5;
        StackRealloc(myStack, koef_capacity, stackErrors);
    }

    if (myStack->size > 0) {
        Elem_t ans = myStack->data[--(myStack->size)];
        myStack->data[myStack->size] = POISONELEMENT;

#ifdef WITH_CANARY_AND_HASHE
        CALCULATE_HASH(myStack);
#endif

        STACK_VERIFY(myStack, stackErrors);
        return ans;
    }
}

void StackDtor(struct Stack* myStack, struct StackErrors* stackErrors)
{
#ifdef WITH_CANARY_AND_HASHE
    Elem_t* allocatedData = (Elem_t *)((Canary_t *)myStack->data - 1);
#else
    Elem_t* allocatedData = myStack->data;
#endif
    free(allocatedData);
    myStack->data = NULL;
    myStack->capacity = 0;
    myStack->size = 0;
}

unsigned int CalculateHash (struct Stack* myStack)
{
    int hash = 0;

    // hash = (hash * HASH_CONST + sizeof(Elem_t) * myStack->capacity) % MOD_FOR_HASH;
    // hash = (hash * HASH_CONST + sizeof(Elem_t) * myStack->size) % MOD_FOR_HASH;
    // hash = (hash * HASH_CONST + sizeof(myStack->data)) % MOD_FOR_HASH;

    for (int i = 0; i < myStack->capacity; i++)
    {
        hash = hash + ((((int)(myStack->data[i] * HASH_CONST)) % MOD_FOR_HASH) ^ XOR_CONST);
    }

    return hash;
}

Elem_t * PointerLeftCanary (struct Stack* myStack)
{
    return (Elem_t *)((Canary_t *)myStack->data - 1);
}

Elem_t * PointerRightCanary (struct Stack* myStack)
{
    return (Elem_t *)((Canary_t *)myStack->data + myStack->capacity);
}

