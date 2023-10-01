#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "log_funcs.h"
#include "stack.h"
#include "error_func.h"

static void StackRealloc(struct Stack *myStack, float koef_capacity);           // why declaration is in .cpp?

void StackDump(struct Stack* myStack, const char *file, int line, const char *function)
{
    fprintf(LOG_FILE, "\nTime is %s\n", __TIME__);
    fprintf(LOG_FILE, "I'm stackDump called from %s (%d) %s\n", function, line, file);
    fprintf(LOG_FILE, "Stack[%p] \"myStack\" from %s(%d) in function - %s.\n\n", myStack->data, FILE, LINE, function);

    fprintf(LOG_FILE, "--------------------------------------------------------------------------\n");
#ifdef WITH_CANARY
    fprintf(LOG_FILE, "STRUCT CANARY_START = %llu\n\n", myStack -> canary_start);
#endif
    fprintf(LOG_FILE, "Struct:\n");
    fprintf(LOG_FILE, "\tsize = %d\n", myStack->size);
    fprintf(LOG_FILE, "\tcapacity = %d\n", myStack->capacity);
    fprintf(LOG_FILE, "\tадрес data[] = %p\n", myStack->data);

#ifdef WITH_CANARY
    fprintf(LOG_FILE, "\nDATA CANARY LEFT  = %llu\n", *PointerLeftCanary(myStack));
#endif

    int now_size = myStack->size;
    for (int i = 0; i < now_size; i++)
    {
        fprintf(LOG_FILE, "\tdata[%d] = %lf\n", i, myStack->data[i]);
    }

#ifdef WITH_CANARY
    fprintf(LOG_FILE, "DATA CANARY RIGHT = %llu\n", *PointerRightCanary(myStack));
    fprintf(LOG_FILE, "HASH = %lld\n", myStack->hash);
    fprintf(LOG_FILE, "\n");
    fprintf(LOG_FILE, "STRUCT CANARY_END = %llu\n", myStack->canary_end);
#endif
    fprintf(LOG_FILE, "--------------------------------------------------------------------------\n");
}


void StackCtor(struct Stack* myStack)
{
    myStack->capacity = CAPACITY;
    myStack->size = 0;

    int scale = 0;

#ifdef WITH_CANARY
    scale = myStack->capacity * sizeof(Elem_t) + 2 * sizeof(Canary_t);
#else
    scale = myStack->capacity * sizeof(Elem_t);
#endif

    myStack->data = (Elem_t *)calloc(scale, sizeof(char));

#ifdef WITH_CANARY
    myStack->data = (Elem_t *)((Canary_t *)myStack->data + 1);

    *(PointerLeftCanary (myStack)) = BUF_CANARY;
    *(PointerRightCanary (myStack)) = BUF_CANARY;

    myStack->canary_start = BUF_CANARY;
    myStack->canary_end   = BUF_CANARY;
#endif

#ifdef WITH_HASH
    CALCULATE_HASH(myStack);
#endif
}

void StackPush(struct Stack* myStack, Elem_t value)
{
    STACK_VERIFY(myStack);

    if (myStack->size >= myStack->capacity) {
        float koef_capacity = UP_KOEF;
        StackRealloc(myStack, koef_capacity);
    }

    myStack->data[myStack->size++] = value;

#ifdef WITH_HASH
    CALCULATE_HASH(myStack);
#endif

    STACK_VERIFY(myStack);
}

static void StackRealloc( Stack *myStack, float koef_capacity)          // hmm, why static?
{
    STACK_VERIFY(myStack);

    myStack->capacity *= koef_capacity;
#ifdef WITH_CANARY
    Elem_t* allocatedData = (Elem_t *)((Canary_t *)myStack->data - 1);
    allocatedData = (Elem_t*)realloc(allocatedData, (myStack->capacity + 2) * sizeof(Elem_t));

    if (allocatedData == NULL)
    {
        printf("ERROR MEMORY FOR REALLOC\n");
    }

    myStack->data = (Elem_t *)((Canary_t *)allocatedData + 1);
#else
    myStack->data = (Elem_t*)realloc(myStack->data, (myStack->capacity + 2) * sizeof(Elem_t));      // what if realloc returns NULL?

    if (myStack->data == NULL)
    {
        printf("ERROR MEMORY FOR REALLOC\n");
    }
#endif

#ifdef WITH_HASH
    CALCULATE_HASH(myStack);
#endif

#ifdef WITH_CANARY                                      // weren't you tired of writing ifdef... each time? this might be done a bit better:            
    *(PointerLeftCanary (myStack)) = BUF_CANARY;        // IF_CANARY(
    *(PointerRightCanary (myStack)) = BUF_CANARY;       //            *(PointerLeftCanary (myStack))  = BUF_CANARY;
#endif                                                  //            *(PointerRightCanary (myStack)) = BUF_CANARY; 
}                                                       //          )
                                                        //
                                                        // where IF_CANARY is a define like this:
                                                        // #ifdef WITH_CANARY
                                                        //      #define IF_CANARY(code) code
                                                        // #else
                                                        //      #define IF_CANARY(code)
                                                        // #endif
                                                        // 
                                                        // similarly with IF_HASH
                                                        //
                                                        // in your code it is possible to rewrite every construction with #else witout it
Elem_t StackPop(struct Stack* myStack)
{
    STACK_VERIFY(myStack);

    if (myStack->size < (myStack->capacity) / UP_KOEF)
    {
        float koef_capacity = DOWN_KOEF;
        StackRealloc(myStack, koef_capacity);
    }

    Elem_t ans = 0;

    if (myStack->size > 0)
    {
        ans = myStack->data[--myStack->size];
        myStack->data[myStack->size] = POISONELEMENT;
    }
    else
    {
        fprintf(LOG_FILE, "SIZE 0, YOU CAN'T GP LOWER\n");
    }
#ifdef WITH_HASH
        CALCULATE_HASH(myStack);
#endif

        STACK_VERIFY(myStack);
        return ans;
}

void StackDtor(struct Stack* myStack)
{
    for (int i = 0; i < myStack->capacity; i++)
    {
        myStack->data[i] = POISONELEMENT;
    }

#ifdef WITH_CANARY
    Elem_t* allocatedData = (Elem_t *)((Canary_t *)myStack->data - 1);
#else
    Elem_t* allocatedData = myStack->data;
#endif

    free(allocatedData);
    myStack->data = NULL;
    myStack->capacity = 0;
    myStack->size = 0;
}

long long CalculateHash(struct Stack* myStack)
{
    long long hash = 0;

    for (int i = 0; i < myStack->capacity; i++)
    {
        hash = hash + ((((long long)(myStack->data[i] * HASH_CONST)) % MOD_FOR_HASH) ^ XOR_CONST);  // just hash += ... :)
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

