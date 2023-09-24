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
        fprintf(stderr, "Ошибка: Превышен размер стека\n");
    }

    if (stackErrors -> ERROR_CAPACITY_BIT)
    {
        fprintf(stderr, "Ошибка: Ошибка вместимости стека (неположительная)\n");
    }

    if (stackErrors -> ERROR_DATA_BIT)
    {
        fprintf(stderr, "Ошибка: Нулевой указатель на данные стека\n");
    }

    if (stackErrors -> ERROR_PUSH_BIT)
    {
        fprintf(stderr, "Ошибка: Ошибка при попытке добавления элемента\n");
    }

    if (stackErrors -> ERROR_CANARY_END_BIT)
    {
        fprintf(stderr, "Ошибка: Ошибка канарейки конца\n");
    }

    if (stackErrors -> ERROR_CANARY_START_BIT)
    {
        fprintf(stderr, "Ошибка: Ошибка канарейки начала\n");
    }

    if (stackErrors -> ERROR_HASH_BIT)
    {
        fprintf(stderr, "Ошибка: Ошибка хэша\n");
    }
}

int StackVerify(struct Stack* myStack, struct StackErrors* stackErrors)
{
    struct StackErrors errorFlags = { 0, 0, 0, 0, 0, 0};

    int sum_error = 0;

    if (myStack->size > myStack->capacity)
    {
        errorFlags.ERROR_SIZE_BIT = 1;
        sum_error++;
    }

    if (myStack->capacity <= 0)
    {
        errorFlags.ERROR_CAPACITY_BIT = 1;
        sum_error++;
    }

    if (myStack->data == NULL)
    {
        errorFlags.ERROR_DATA_BIT = 1;
        sum_error++;
    }

    if (myStack->canary_start != BUF_CANARY)
    {
        errorFlags.ERROR_CANARY_START_BIT = 1;
        sum_error++;
    }

    if (myStack->canary_start != BUF_CANARY)
    {
        errorFlags.ERROR_CANARY_END_BIT = 1;
        sum_error++;
    }

    unsigned int new_hash = CalculateHash (myStack);
    if (myStack->hash != new_hash) {
        errorFlags.ERROR_HASH_BIT = 1;
        sum_error++;
    }

    return sum_error;
}


