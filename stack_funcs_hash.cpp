#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "stack_funcs_hash.h"
#include "error_func.h"

void StackDump(struct Stack* myStack, struct StackErrors* stackErrors, const char *file, int line, const char *function)
{
    fprintf(LOG_FILE, "\nTime is %s\n", __TIME__);
    fprintf(LOG_FILE, "I'm stackDump called from %s (%d) %s\n", function, line, file);
    fprintf(LOG_FILE, "Stack[%p] \"myStack\" from %s(%d) in function - %s.\n\n", myStack->data, FILE, LINE, function);

    fprintf(LOG_FILE, "size = %d\n", myStack->size);
    fprintf(LOG_FILE, "capacity = %d\n", myStack->capacity);
    fprintf(LOG_FILE, "адрес data[] = %p\n", myStack->data);

#ifdef WITH_CANARY_AND_HASHE
    fprintf(LOG_FILE, "DATA CANARY = %llu\n", myStack->data[0]);
#endif

    int now_size = myStack->size;

    #ifdef WITH_CANARY_AND_HASHE
    for (int i = 1; i <= now_size; i++) {
        fprintf(LOG_FILE, "data[%d] = %lf\n", i, myStack->data[i]);
    }
    #else
    for (int i = 0; i < now_size; i++) {
        fprintf(LOG_FILE, "data[%d] = %lf\n", i, myStack->data[i]);
    }
    #endif

    #ifdef WITH_CANARY_AND_HASHE
    fprintf(LOG_FILE, "DATA CANARY = %llu\n", myStack->data[myStack->capacity + 1]);

    fprintf(LOG_FILE, "\n");
    #endif
}

void StackCtor(struct Stack* myStack, struct StackErrors* stackErrors)
{
    myStack->capacity = Capacity;
    #ifdef WITH_CANARY_AND_HASHE
    myStack->data = (Elem_t*)calloc(myStack->capacity + 2, sizeof(Elem_t)); // todo
    #else
    myStack->data = (Elem_t*)calloc(myStack->capacity, sizeof(Elem_t));
    #endif

    #ifdef WITH_CANARY_AND_HASHE
    *(SolveLeftCanary (myStack)) = BUF_CANARY;
    *(SolveRightCanary (myStack)) = BUF_CANARY;

    myStack -> canary_start = BUF_CANARY;
    myStack -> canary_end = BUF_CANARY;

    myStack->hash = CalculateHash(myStack); // Рассчитываем и сохраняем хэш-код
    #endif

    myStack->size = 0;

    STACK_VERIFY(myStack, stackErrors);

    #ifdef WITH_CANARY_AND_HASHE
    fprintf(LOG_FILE, "\nCalculateHash = %u\n", CalculateHash(myStack));
    fprintf(LOG_FILE, "myStack->hash = %u\n\n", myStack->hash);
    #endif
}

void StackPush(struct Stack* myStack, Elem_t value, struct StackErrors* stackErrors)
{
    #ifdef WITH_CANARY_AND_HASHE
    myStack -> hash = CalculateHash (myStack);
    #endif
    STACK_VERIFY(myStack, stackErrors);

    #ifdef WITH_CANARY_AND_HASHE
    if (myStack->size + 1 >= myStack->capacity) {
        float koef_capacity = 2.0;
        StackRellocUp(myStack, koef_capacity, stackErrors);
    }
    #else
    if (myStack->size >= myStack->capacity) {
        float koef_capacity = 2.0;
        StackRellocUp(myStack, koef_capacity, stackErrors);
    }
    #endif

    else if (myStack->size < (myStack->capacity) / 2)
    {
        float koef_capacity = 0.5;
        StackRellocUp(myStack, koef_capacity, stackErrors);
    }

    if (myStack->size == SIZE)
    {
        myStack->data[myStack->size] = value;
    }

    else
    {
        myStack->data[myStack->size++] = value;
    }

    #ifdef WITH_CANARY_AND_HASHE
    myStack->hash = CalculateHash(myStack);
    #endif

    STACK_VERIFY(myStack, stackErrors);
}

void StackRellocUp(struct Stack *myStack, float koef_capacity, struct StackErrors* stackErrors)
{
    STACK_VERIFY(myStack, stackErrors);

    fprintf(LOG_FILE, "я - StackRellocUp\n");
    fprintf(LOG_FILE, "capacity!!!перед изменением = %d\n", myStack->capacity);
    myStack->capacity *= koef_capacity;
    fprintf(LOG_FILE, "capacity!!!после изменений = %d\n", myStack->capacity);
    myStack->data = (Elem_t*)realloc(myStack->data, (myStack->capacity + 2) * sizeof(Elem_t));

    #ifdef WITH_CANARY_AND_HASHE
    *(SolveLeftCanary (myStack)) = BUF_CANARY;
    *(SolveRightCanary (myStack)) = BUF_CANARY;
    #endif
}

Elem_t StackPop(struct Stack* myStack, struct StackErrors* stackErrors)
{
    #ifdef WITH_CANARY_AND_HASHE
    myStack->hash = CalculateHash(myStack);
#endif
    STACK_VERIFY(myStack, stackErrors);

#ifdef WITH_CANARY_AND_HASHE
    if (myStack->size >= 0) {
        Elem_t ans1 = myStack->data[myStack->size--];
        myStack->data[myStack->size + 1] = REZET; // Обнуляем только последний элемент
        return ans1;
    }
#else
    if (myStack->size > 0) {
        Elem_t ans = myStack->data[--(myStack->size)];
        myStack->data[myStack->size] = REZET; // Обнуляем только последний элемент
        return ans;
    }
#endif


    #ifdef WITH_CANARY_AND_HASHE
    myStack -> hash = CalculateHash (myStack);
    #endif
    STACK_VERIFY(myStack, stackErrors);

}

void StackDtor(struct Stack* myStack, struct StackErrors* stackErrors)
{
    free(myStack->data);
    myStack->data = NULL;
    myStack->capacity = 0;
    myStack->size = 0;
}

unsigned int CalculateHash (struct Stack* myStack)
{
    const char* data = (const char*)myStack->data;
    unsigned int hash = 1;

    hash = (hash * HASH_CONST + sizeof(myStack->capacity)) % MOD_FOR_HASH;
    hash = (hash * HASH_CONST + sizeof(Elem_t) * myStack->size) % MOD_FOR_HASH;

    for (int i = 0; i < myStack->size; i++) {
        hash = (hash * HASH_CONST + data[i]) % MOD_FOR_HASH;
    }

    return hash;
}

unsigned long long* SolveLeftCanary (struct Stack* myStack)
{
    return (unsigned long long *)myStack->data - 3;
}

unsigned long long* SolveRightCanary (struct Stack* myStack)
{
    return (unsigned long long *)(myStack->data + myStack->capacity + 1);
}

