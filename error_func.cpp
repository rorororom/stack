#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "stack.h"
#include "error_func.h"

void PrintStackErrors(int sum_errors)
{
    if (sum_errors & ERROR_SIZE_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Превышен размер стека\n");
    }

    if (sum_errors & ERROR_CAPACITY_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка вместимости стека (неположительная)\n");
    }

    if (sum_errors & ERROR_DATA_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Нулевой указатель на данные стека\n");
    }
#ifdef WITH_CANARY

    if (sum_errors & ERROR_PUSH_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка при попытке добавления элемента\n");
    }

    if (sum_errors & ERROR_CANARY_END_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка канарейки конца\n");
    }

    if (sum_errors & ERROR_CANARY_START_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка канарейки начала\n");
    }
#endif

#ifdef WITH_HASH
    if (sum_errors & ERROR_HASH_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка хэша\n");
        fprintf(LOG_FILE, "-------------------------\n");
    }
#endif
}

int StackVerify(struct Stack* myStack)
{
    int sum_errors = 0;

    if (myStack->size > myStack->capacity)
    {
        printf("A\n");                              // nice printf
        sum_errors = sum_errors | ERROR_SIZE_BIT;
    }

    if (myStack->capacity <= 0)
    {
        printf("AA\n");
        sum_errors = sum_errors | ERROR_CAPACITY_BIT;
    }

    if (myStack->data == NULL)
    {
        printf("AAA\n");
        sum_errors = sum_errors | ERROR_DATA_BIT;
    }

#ifdef WITH_HASH
    unsigned long long new_hash = CalculateHash (myStack);
    if (myStack->hash != new_hash) {
        printf("AAAA\n");
        sum_errors = sum_errors | ERROR_HASH_BIT;
    }
#endif

    if (*(PointerLeftCanary (myStack)) != BUF_CANARY){
        printf("AAAAA\n");
        sum_errors = sum_errors | ERROR_DATA_START_BIT;
    }

#ifdef WITH_CANARY
    if (*(PointerRightCanary (myStack)) != BUF_CANARY) {
        printf("AAAAAA\n");
        sum_errors = sum_errors | ERROR_DATA_END_BIT;
    }
#endif

    if (sum_errors > 0) // not > 0 but != 0! if you set the senior meaning bit in int variable to 1, it becomes negative and this if won't work, but you want it 
    {                   //                                                                                                                 to work in this case...
        PrintStackErrors(sum_errors);
    }

    return sum_errors;
}


