#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "stack_funcs_hash.h"

#define LINE __LINE__
#define FILE __FILE__

typedef int Elem_t; // тип элементов стека

int StackFuncHash ()
{
    Stack myStack = {
        NULL,
        0,
        0
    };

    struct StackErrors stackErrors = {
        (1 << 0), // ERROR_SIZE_BIT
        (1 << 1), // ERROR_CAPACITY_BIT
        (1 << 2), // ERROR_DATA_BIT
        (1 << 3), // ERROR_PUSH_BIT
        (1 << 4)  // ERROR_HASH_BIT
    };

    StackCtor(&myStack, &stackErrors);
    for (int i = 1; i <= 10; i++) {
        StackPush(&myStack, i * 1, &stackErrors);
    }

    StackDump (&myStack, &stackErrors);

    int size = myStack.size;
    for (int i = 0; i < size; i++) {
        fprintf(LOG_FILE, "%d\n", StackPop(&myStack, &stackErrors));
    }

    StackDtor (&myStack, &stackErrors);

    return 0;
}

void StackRellocUp(struct Stack *myStack, float koef_capacity, struct StackErrors* stackErrors)
{
    *stackErrors = StackOk(myStack);

    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT || stackErrors -> ERROR_DATA_BIT) {
        PrintStackErrors(stackErrors);
    }

    fprintf(LOG_FILE, "capacity!!!перед изменением = %d\n", myStack->capacity);
    myStack->capacity *= koef_capacity;
    fprintf(LOG_FILE, "capacity!!!после изменений = %d\n", myStack->capacity);
    myStack->data = (Elem_t*)realloc(myStack->data, sizeof(Elem_t) * myStack->capacity);
}

void StackDump(struct Stack* myStack, struct StackErrors* stackErrors)
{
    *stackErrors = StackOk(myStack);

    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT || stackErrors -> ERROR_DATA_BIT) {
        PrintStackErrors(stackErrors);
    }

    fprintf(LOG_FILE, "\nTime is %s\n", __TIME__);
    fprintf(LOG_FILE, "Stack[%p] \"myStack\" from %s(%d) in function - %s.\n", myStack->data, FILE, LINE, __func__);

    int now_size = myStack->size;
    for (int i = 0; i < now_size; i++) {
        fprintf(LOG_FILE, "data[%d] = %d\n", i, myStack->data[i]);
    }

    fprintf(LOG_FILE, "size = %d\n", myStack->size);
    fprintf(LOG_FILE, "capacity = %d\n", myStack->capacity);
    fprintf(LOG_FILE, "data[%p]\n", myStack->data);
}

void PrintStackErrors(struct StackErrors* stackErrors)
{
    if (stackErrors -> ERROR_SIZE_BIT) {
        fprintf(stderr, "Ошибка: Превышен размер стека\n");
    }
    if (stackErrors -> ERROR_CAPACITY_BIT) {
        fprintf(stderr, "Ошибка: Ошибка вместимости стека (неположительная)\n");
    }
    if (stackErrors -> ERROR_DATA_BIT) {
        fprintf(stderr, "Ошибка: Нулевой указатель на данные стека\n");
    }
    if (stackErrors -> ERROR_PUSH_BIT) {
        fprintf(stderr, "Ошибка: Ошибка при попытке добавления элемента\n");
    }
}

struct StackErrors StackOk(struct Stack* myStack)
{
    struct StackErrors errorFlags = { 0, 0, 0, 0, 0 };

    if (myStack->size > myStack->capacity) {
        errorFlags.ERROR_SIZE_BIT = 1;
    }

    if (myStack->capacity <= 0) {
        errorFlags.ERROR_CAPACITY_BIT = 1;
    }

    if (myStack->data == NULL) {
        errorFlags.ERROR_DATA_BIT = 1;
    }

    unsigned int new_hash = CalculateHash (myStack);
    if (myStack->hash != new_hash) {
        errorFlags.ERROR_HASH_BIT = 1;
    }

    return errorFlags;
}

void StackCtor(struct Stack* myStack, struct StackErrors* stackErrors)
{
    myStack->data = (Elem_t*)calloc(Capacity, sizeof(Elem_t));
    myStack->capacity = Capacity;
    myStack->size = 0;
    myStack->hash = CalculateHash(myStack); // Рассчитываем и сохраняем хэш-код
}

void StackPush(struct Stack* myStack, Elem_t value, struct StackErrors* stackErrors)
{
    *stackErrors = StackOk(myStack);

    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT || stackErrors -> ERROR_DATA_BIT) {
        PrintStackErrors(stackErrors);
    }

    if (myStack->size >= myStack->capacity) {
        float koef_capacity = 2.0;
        StackRellocUp(myStack, koef_capacity, stackErrors);
    }

    else if (myStack->size <= myStack->capacity / 2)
    {
        float koef_capacity = 0.5;
        StackRellocUp(myStack, koef_capacity, stackErrors);
    }

    myStack->data[myStack->size++] = value;
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
    *stackErrors = StackOk(myStack);

    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT || stackErrors -> ERROR_DATA_BIT) {
        PrintStackErrors(stackErrors);
    }

    return myStack->data[--myStack->size];
}

unsigned int CalculateHash (const struct Stack* myStack)
{
    const char* data = (const char*)myStack->data;
    unsigned int hash = 0;

    for (int i = 0; i < myStack->size; i++) {
        hash += (unsigned int)data[i];
    }

    return hash;
}
