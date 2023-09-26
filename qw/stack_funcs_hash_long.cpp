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

int StackFuncHash()
{
    Stack myStack = {
        0,
        NULL,
        0,
        0,
        0,
        0
    };

    myStack.canary_start = BUF_CANARY;
    myStack.canary_end = BUF_CANARY;

    struct StackErrors stackErrors = {};

    StackCtor(&myStack, &stackErrors);

    for (int i = 1; i <= SIZE; i++)
    {
        long long knr = *(long long*)myStack.data;
        printf ("первая канарейка 1- %lu\n", knr);
        StackPush(&myStack, i * 10, &stackErrors);
    }

   int size = myStack.size;
    fprintf(LOG_FILE, "\nЯ ЗАХОЖУ В ПОП\n");
    for (int i = 0; i < size; i++)
    {
        myStack.hash = CalculateHash(&myStack);
        fprintf(LOG_FILE, "%lld\n", StackPop(&myStack, &stackErrors));  // Изменено на %lld
    }

    StackDtor(&myStack, &stackErrors);

    return 0;
}

void StackRellocUp(struct Stack* myStack, float koef_capacity, struct StackErrors* stackErrors)
{
    fprintf(LOG_FILE, "я - StackRellocUp1\n");
    STACK_VERIFY(myStack, stackErrors);

    fprintf(LOG_FILE, "я - StackRellocUp\n");
    fprintf(LOG_FILE, "capacity!!!перед изменением = %d\n", myStack->capacity);
    myStack->capacity *= koef_capacity;
    fprintf(LOG_FILE, "capacity!!!после изменений = %d\n", myStack->capacity);

    // Используем realloc для изменения размера массива данных
    myStack->data = (Elem_t*)realloc(myStack->data, sizeof(Elem_t) * myStack->capacity + 2 * sizeof(long long));

    // Устанавливаем новые значения канареек
    *(long long*)myStack->data = BUF_CANARY;
    *(long long*)((char*)myStack->data + sizeof(long long) + sizeof(Elem_t) * myStack->capacity) = BUF_CANARY;
}


void StackDump(struct Stack* myStack, struct StackErrors* stackErrors, const char* file, int line, const char* function)
{
    fprintf(LOG_FILE, "\nTime is %s\n", __TIME__);
    fprintf(LOG_FILE, "ОШИБКААААААААААААА\n I'm stackDump called from %s (%d) %s\n", function, line, file);
    fprintf(LOG_FILE, "Stack[%p] \"myStack\" from %s(%d) in function - %s.\n", myStack->data, FILE, LINE, function);

    int now_size = myStack->size;

    for (int i = 0; i < now_size; i++)
    {
        fprintf(LOG_FILE, "data[%d] = %lld\n", i, myStack->data[i]);  // Изменено на %lld
    }

    fprintf(LOG_FILE, "size = %d\n", myStack->size);
    fprintf(LOG_FILE, "capacity = %d\n", myStack->capacity);
    fprintf(LOG_FILE, "data[%p]\n", myStack->data);
}

void StackCtor(struct Stack* myStack, struct StackErrors* stackErrors)
{
    myStack->capacity = Capacity;
    myStack->data = (Elem_t*)calloc(myStack->capacity * sizeof(Elem_t) + 2 * sizeof(long long), sizeof(char));  // Изменено на sizeof(long long)
    *(long long*)myStack->data = BUF_CANARY;  // Изменено на long long
    //printf ("канарейка начала - %lu\n", *(long long*)myStack->data);
    *(long long*)((char*)myStack->data + sizeof(long long) + sizeof(Elem_t) * myStack->capacity) = BUF_CANARY;  // Изменено на long long
    //printf ("канарейка начала - %lu\n", *(long long*)((char*)myStack->data + sizeof(long long) + sizeof(Elem_t) * myStack->capacity));

    fprintf(LOG_FILE, "я в сторе\n");

    myStack->size = 0;
    myStack->hash = CalculateHash(myStack); // Рассчитываем и сохраняем хэш-код

    STACK_VERIFY(myStack, stackErrors);

    fprintf(LOG_FILE, "CalculateHash = %u\n", CalculateHash(myStack));
    fprintf(LOG_FILE, "myStack->hash = %u\n", myStack->hash);
}

void StackPush(struct Stack* myStack, Elem_t value, struct StackErrors* stackErrors)
{
    printf ("первая канарейка - %lu\n", *(long long*)myStack->data);
    STACK_VERIFY(myStack, stackErrors);

    if (myStack->size >= myStack->capacity)
    {
        float koef_capacity = 2.0;
        StackRellocUp(myStack, koef_capacity, stackErrors);
    }

    else if (myStack->size < (myStack->capacity) / 2)
    {
        float koef_capacity = 0.5;
        StackRellocUp(myStack, koef_capacity, stackErrors);
    }

    fprintf (LOG_FILE, "я в пуше\n");
    printf ("первая канарейка2 - %lu\n", *(long long*)myStack->data);

    myStack->data[1 + (myStack->size)] = value;
    printf ("первая канарейка3 - %lu\n", *(long long*)myStack->data);

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
    return myStack->data[--(myStack->size)];
}

unsigned int CalculateHash(struct Stack* myStack)
{
    const char* data = (const char*)myStack->data;
    unsigned int hash = 0;

    for (int i = 0; i < myStack->size; i++)
    {
        hash = (hash * HASH_CONST + data[i]) % MOD_FOR_HASH;
    }

    return hash;
}

Elem_t* ReturnData(struct Stack* myStack)
{
    return (Elem_t*)(myStack->data + sizeof(long long));  // Изменено на sizeof(long long)
}

