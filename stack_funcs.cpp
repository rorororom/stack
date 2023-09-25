#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "stack_funcs.h"

#define LINE __LINE__
#define FILE __FILE__


void StackFunc()
{
    struct StackSimple myStack = {
        NULL,
        0,
        0
    };

    struct StackErrorsSimple stackErrors = { };

    StackCtorSimple(&myStack, &stackErrors);  // Передаем указатель на структуру myStack
    for (int i = 1; i <= 10; i++) {
        StackPushSimple(&myStack, i * 1, &stackErrors);  // Передаем указатель на структуру myStack
    }

    StackDumpSimple(&myStack, &stackErrors);  // Передаем указатель на структуру myStack

    int size = myStack.size;  // Обращаемся к полю size без "->"
    for (int i = 0; i < size; i++) {
        fprintf(LOG_FILE, "%d\n", StackPopSimple(&myStack, &stackErrors));  // Передаем указатель на структуру myStack
    }

    StackDtorSimple(&myStack, &stackErrors);  // Передаем указатель на структуру myStack
}


void StackRellocUpSimple(struct StackSimple *myStack, float koef_capacity, struct StackErrorsSimple* stackErrors)
{
    *stackErrors = StackOkSimple(myStack);

    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT || stackErrors -> ERROR_DATA_BIT) {
        PrintStackErrorsSimple(stackErrors);
    }

    fprintf(LOG_FILE, "capacity!!!перед изменением = %d\n", myStack->capacity);
    myStack->capacity *= koef_capacity;
    fprintf(LOG_FILE, "capacity!!!после изменений = %d\n", myStack->capacity);
    myStack->data = (Elem_t*)realloc(myStack->data, sizeof(Elem_t) * myStack->capacity);
}

void StackDumpSimple(struct StackSimple* myStack, struct StackErrorsSimple* stackErrors)
{
    *stackErrors = StackOkSimple(myStack);

    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT || stackErrors -> ERROR_DATA_BIT) {
        PrintStackErrorsSimple(stackErrors);
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

void PrintStackErrorsSimple(struct StackErrorsSimple* stackErrors)
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

struct StackErrorsSimple StackOkSimple(struct StackSimple* myStack)
{
    struct StackErrorsSimple errorFlags = { 0, 0, 0, 0 };

    if (myStack->size > myStack->capacity) {
        errorFlags.ERROR_SIZE_BIT = 1;
    }

    if (myStack->capacity <= 0) {
        errorFlags.ERROR_CAPACITY_BIT = 1;
    }

    if (myStack->data == NULL) {
        errorFlags.ERROR_DATA_BIT = 1;
    }

    return errorFlags;
}

void StackCtorSimple(struct StackSimple* myStack, struct StackErrorsSimple* stackErrors)
{
    myStack->data = (Elem_t*)calloc(Capacity_stack, sizeof(Elem_t));
    myStack->capacity = Capacity_stack;
    myStack->size = 0;
}

void StackPushSimple(struct StackSimple* myStack, Elem_t value, struct StackErrorsSimple* stackErrors)
{
    *stackErrors = StackOkSimple(myStack);

    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT || stackErrors -> ERROR_DATA_BIT) {
        PrintStackErrorsSimple(stackErrors);
    }

    if (myStack->size >= myStack->capacity) {
        float koef_capacity = 2.0;
        StackRellocUpSimple(myStack, koef_capacity, stackErrors);
    }

    else if (myStack->size <= myStack->capacity / 2)
    {
        float koef_capacity = 0.5;
        StackRellocUpSimple(myStack, koef_capacity, stackErrors);
    }

    myStack->data[myStack->size++] = value;
}

void StackDtorSimple(struct StackSimple* myStack, struct StackErrorsSimple* stackErrors)
{
    free(myStack->data);
    myStack->data = NULL;
    myStack->capacity = 0;
    myStack->size = 0;
}

Elem_t StackPopSimple(struct StackSimple* myStack, struct StackErrorsSimple* stackErrors)
{
    *stackErrors = StackOkSimple(myStack);

    if (stackErrors -> ERROR_SIZE_BIT || stackErrors -> ERROR_CAPACITY_BIT || stackErrors -> ERROR_DATA_BIT) {
        PrintStackErrorsSimple(stackErrors);
    }

    return myStack->data[--myStack->size];
}



