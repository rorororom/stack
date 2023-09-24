#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "stack_funcs_hash.h"
#include "error_func.h"

void PrintStackErrors(struct StackErrors* stackErrors)
{
    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT || stackErrors -> ERROR_DATA_BIT) {
        PrintStackErrors(stackErrors);
    }

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

    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT ||
        stackErrors -> ERROR_DATA_BIT || stackErrors -> ERROR_CANARY_END_BIT ||
        stackErrors -> ERROR_CANARY_END_BIT || stackErrors -> ERROR_HASH_BIT ||
        stackErrors -> ERROR_PUSH_BIT)
    {
        PrintStackErrors(stackErrors);
    }
}

struct StackErrors StackOk(struct Stack* myStack)
{
    struct StackErrors errorFlags = { 0, 0, 0, 0, 0, 0};

    if (myStack->size > myStack->capacity)
    {
        errorFlags.ERROR_SIZE_BIT = 1;
    }

    if (myStack->capacity <= 0)
    {
        errorFlags.ERROR_CAPACITY_BIT = 1;
    }

    if (myStack->data == NULL)
    {
        errorFlags.ERROR_DATA_BIT = 1;
    }

    if (myStack->canary_start != Start)
    {
        errorFlags.ERROR_CANARY_START_BIT = 1;
    }

    if (myStack->canary_start != End)
    {
        errorFlags.ERROR_CANARY_END_BIT = 1;
    }

    unsigned int new_hash = CalculateHash (myStack);
    if (myStack->hash != new_hash) {
        errorFlags.ERROR_HASH_BIT = 1;
    }

    return errorFlags;
}


