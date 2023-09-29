typedef double Elem_t;
typedef unsigned long long Canary_t;

const int SIZE = 10;
const int Capacity = SIZE + 0;
const int HASH_CONST = 234;
const int MOD_FOR_HASH = 107;
const Elem_t POISONELEMENT = -1;
const int DOP_RIGHT = 1;
const int DOP_LEFT = 1;

//const uint32_t Start = 0xDEADBEEF;
//const uint32_t End = 0xFEEDBEEF;
const unsigned long long BUF_CANARY =  0xDEDEEDED;

enum eroor {
    ERROR = -1
};

#define LINE __LINE__
#define FILE __FILE__

#define STACK_DUMP(myStack, stackErrors) StackDump(myStack, stackErrors, FILE, LINE, __func__);

#define STACK_VERIFY(myStack, stackErrors) {        \
    if (StackVerify(myStack, stackErrors) > 0) {    \
        PrintStackErrors(stackErrors);              \
        STACK_DUMP(myStack, stackErrors);           \
    }                                               \
}

#define CALCULATE_HASH(stack) stack->hash = CalculateHash(stack)

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
#ifdef WITH_CANARY_AND_HASHE
    int ERROR_HASH_BIT;
    int ERROR_CANARY_START_BIT;
    int ERROR_CANARY_END_BIT;
    int ERROR_DATA_END_BIT;
    int ERROR_DATA_START_BIT;
#endif
};

void StackRealloc(struct Stack *myStack, float koef_capacity, struct StackErrors* stackErrors);
void StackDump(struct Stack* myStack, struct StackErrors* stackErrors, const char *file, int line, const char *function);
struct StackErrors StackOk(struct Stack* myStack);
void StackCtor(struct Stack* myStack, struct StackErrors* stackErrors);
void StackPush(struct Stack* myStack, Elem_t value, struct StackErrors* stackErrors);
void StackDtor(struct Stack* myStack, struct StackErrors* stackErrors);
Elem_t StackPop(struct Stack* myStack, struct StackErrors* stackErrors);
unsigned int CalculateHash(struct Stack* myStack);
void PrintStackErrors(struct StackErrors* stackErrors);
Elem_t * PointerLeftCanary(struct Stack* myStack);
Elem_t * PointerRightCanary(struct Stack* myStack);
