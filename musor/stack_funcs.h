typedef double Elem_t; // тип элементов стека

const int Capacity_stack = 100;

struct StackSimple {
    Elem_t* data;
    int capacity; // объем
    int size; // номер заполненного
};

struct StackErrorsSimple {
    int ERROR_SIZE_BIT;
    int ERROR_CAPACITY_BIT;
    int ERROR_DATA_BIT;
    int ERROR_PUSH_BIT;
};

void PrintStackErrorsSimple(struct StackErrorsSimple* stackErrors);
void StackRellocUpSimple(struct StackSimple *myStack, float koef_capacity, struct StackErrorsSimple* stackErrors);
void StackDumpSimple(struct StackSimple* myStack, struct StackErrorsSimple* stackErrors);
struct StackErrorsSimple StackOkSimple(struct StackSimple* myStack);
void StackCtorSimple(struct StackSimple* myStack, struct StackErrorsSimple* stackErrors);
void StackPushSimple(struct StackSimple* myStack, Elem_t value, struct StackErrorsSimple* stackErrors);
void StackDtorSimple(struct StackSimple* myStack, struct StackErrorsSimple* stackErrors);
Elem_t StackPopSimple(struct StackSimple* myStack, struct StackErrorsSimple* stackErrors);
void StackFunc ();
