#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "stack.h"
#include "error_func.h"

void PrintStackErrors(struct StackErrors* stackErrors)
{
    if (stackErrors -> ERROR_SIZE_BIT)
    {
        printf("Ошибка: Превышен размер стека\n");
    }

    if (stackErrors -> ERROR_CAPACITY_BIT)
    {
        printf("Ошибка: Ошибка вместимости стека (неположительная)\n");
    }

    if (stackErrors -> ERROR_DATA_BIT)
    {
        printf("Ошибка: Нулевой указатель на данные стека\n");
    }
#ifdef WITH_CANARY_AND_HASHE

    if (stackErrors -> ERROR_PUSH_BIT)
    {
        printf("Ошибка: Ошибка при попытке добавления элемента\n");
    }

    if (stackErrors -> ERROR_CANARY_END_BIT)
    {
        printf("Ошибка: Ошибка канарейки конца\n");
    }

    if (stackErrors -> ERROR_CANARY_START_BIT)
    {
        printf("Ошибка: Ошибка канарейки начала\n");
    }

    if (stackErrors -> ERROR_HASH_BIT)
    {
        printf("Ошибка: Ошибка хэша\n");
    }
#endif
}

int StackVerify(struct Stack* myStack, struct StackErrors* stackErrors)
{
    int sum_error = 0;

    if (myStack->size > myStack->capacity)
    {
        stackErrors->ERROR_SIZE_BIT = 1;
        sum_error++;
    }

    if (myStack->capacity <= 0)
    {
        stackErrors->ERROR_CAPACITY_BIT = 1;
        sum_error++;
    }

    if (myStack->data == NULL)
    {
        stackErrors->ERROR_DATA_BIT = 1;
        sum_error++;
    }

#ifdef WITH_CANARY_AND_HASHE
    unsigned int new_hash = CalculateHash (myStack);
    if (myStack->hash != new_hash) {
        stackErrors->ERROR_HASH_BIT = 1;
        sum_error++;
    }

    if (*(PointerLeftCanary (myStack)) != BUF_CANARY){
        stackErrors->ERROR_DATA_START_BIT = 1;
        sum_error++;
    }

    if (*(PointerRightCanary (myStack)) != BUF_CANARY) {
        stackErrors->ERROR_DATA_END_BIT = 1;
        sum_error++;
    }
#endif

    return sum_error;
}


