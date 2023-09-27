#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "stack_funcs_hash.h"
#include "error_func.h"

void PrintStackErrors(struct StackErrors* stackErrors)
{
    if (stackErrors -> ERROR_SIZE_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Превышен размер стека\n");
    }

    if (stackErrors -> ERROR_CAPACITY_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка вместимости стека (неположительная)\n");
    }

    if (stackErrors -> ERROR_DATA_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Нулевой указатель на данные стека\n");
    }
#ifdef WITH_CANARY_AND_HASHE
    if (stackErrors -> ERROR_PUSH_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка при попытке добавления элемента\n");
    }

    if (stackErrors -> ERROR_CANARY_END_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка канарейки конца\n");
    }

    if (stackErrors -> ERROR_CANARY_START_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка канарейки начала\n");
    }

    if (stackErrors -> ERROR_HASH_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка хэша\n");
    }
#endif
}

int StackVerify(struct Stack* myStack, struct StackErrors* stackErrors)
{
    struct StackErrors errorFlags = { 0, 0, 0, 0, 0, 0 };

    int sum_error = 0;

    //printf ("myStack->size - %d\n", myStack->size);
    //printf ("myStack->capacity - %d\n", myStack->capacity);
    //printf ("\n");
    if (myStack->size > myStack->capacity)
    {
        errorFlags.ERROR_SIZE_BIT = 1;
        sum_error++;
        printf ("HERE1\n");
    }

    //if (myStack -> capacity <= 0)
    //{
    //printf ("myStack->capacity - %d\n", myStack->capacity);
    //printf ("\n");
    //}
    if (myStack->capacity <= 0)
    {
        errorFlags.ERROR_CAPACITY_BIT = 1;
        sum_error++;
        printf ("HERE2\n");
    }

    // if (myStack -> data == NULL)
    // {
    // printf ("myStack->data - %p\n", &myStack->capacity);
    // printf ("\n");
    // }
    if (myStack->data == NULL)
    {
        errorFlags.ERROR_DATA_BIT = 1;
        sum_error++;
        printf ("HERE3\n");
    }

    // if (myStack -> canary_start != BUF_CANARY)
    // {
    // printf ("canary_start - %d\n", myStack -> canary_start);
    // printf ("\n");
    // }
#ifdef WITH_CANARY_AND_HASHE
    if (myStack->canary_start != BUF_CANARY)
    {
        errorFlags.ERROR_CANARY_START_BIT = 1;
        sum_error++;
        printf ("HERE4\n");
    }

    // if (myStack -> canary_end != BUF_CANARY)
    // {
    // printf ("canary_end - %d\n", myStack -> canary_end);
    // printf ("\n");
    // }
    if (myStack->canary_end != BUF_CANARY)
    {
        errorFlags.ERROR_CANARY_END_BIT = 1;
        sum_error++;
        printf ("HERE5\n");
    }

    unsigned int new_hash = CalculateHash (myStack);
    // printf ("new_hashe - %d\n", new_hash);
    // printf ("myStack->hash - %d\n", myStack->hash);
    // printf ("\n");
    if (myStack->hash != new_hash) {
        errorFlags.ERROR_HASH_BIT = 1;
        sum_error++;
        printf ("HERE6\n");
        printf ("new_hashe - %d\n", new_hash);
        printf ("myStack->hash - %d\n", myStack->hash);
        printf ("\n");
    }

    //printf ("первая канарейка - %lu\n", *(long long *)myStack->data);
    if (*(SolveLeftCanary (myStack)) != BUF_CANARY){
        errorFlags.ERROR_DATA_START_BIT = 1;
        sum_error++;
        printf ("HERE7\n");
    }

    //printf ("вторая канарейка - %lu\n", *(long long*)((char*) myStack->data + sizeof(BUF_CANARY) + sizeof(Elem_t) * myStack->capacity));
    if (*(SolveRightCanary (myStack)) != BUF_CANARY) {
        errorFlags.ERROR_DATA_END_BIT = 1;
        sum_error++;
        printf ("HERE8\n");
    }
#endif

    return sum_error;
}


