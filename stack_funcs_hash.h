typedef int Elem_t; // тип элементов стека

const int Capacity = 100;
const int HASH_CONST = 234;
const int MOD_FOR_HASH = 107;
const int SIZE = 5;

//const uint32_t Start = 0xDEADBEEF;
//const uint32_t End = 0xFEEDBEEF;
const long long BUF_CANARY =  0xDEADBEEF;

enum eroor {
    ERROR = -1
};

struct Stack {
    uint32_t canary_start;
    Elem_t* data;
    int capacity; // объем
    int size; // номер заполненного
    unsigned int hash;
    uint32_t canary_end;
};

struct StackErrors {
    int ERROR_SIZE_BIT;
    int ERROR_CAPACITY_BIT;
    int ERROR_DATA_BIT;
    int ERROR_PUSH_BIT;
    int ERROR_HASH_BIT;
    int ERROR_CANARY_START_BIT;
    int ERROR_CANARY_END_BIT;
    int ERROR_DATA_END_BIT;
    int ERROR_DATA_START_BIT;
};

void StackRellocUp(struct Stack *myStack, float koef_capacity, struct StackErrors* stackErrors);
void StackDump(struct Stack* myStack, struct StackErrors* stackErrors, const char *file, int line, const char *function);
struct StackErrors StackOk(struct Stack* myStack);
void StackCtor(struct Stack* myStack, struct StackErrors* stackErrors);
void StackPush(struct Stack* myStack, Elem_t value, struct StackErrors* stackErrors);
void StackDtor(struct Stack* myStack, struct StackErrors* stackErrors);
Elem_t StackPop(struct Stack* myStack, struct StackErrors* stackErrors);
int StackFuncHash ();
unsigned int CalculateHash (struct Stack* myStack);
Elem_t* ReturnData (struct Stack* myStack);
void PrintStackErrors(struct StackErrors* stackErrors);
