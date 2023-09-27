typedef double Elem_t; // тип элементов стека

const int Capacity = 100;
const int HASH_CONST = 234;
const int MOD_FOR_HASH = 107;
const int SIZE = 5;
const Elem_t REZET = -1;
const int DOP_RIGHT = 1;
const int DOP_LEFT = 3;

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

struct Stack {
#ifdef WITH_CANARY_AND_HASHE
    uint32_t canary_start;
#endif
    Elem_t* data;
    int capacity; // объем
    int size; // номер заполненного
#ifdef WITH_CANARY_AND_HASHE
    unsigned int hash;
    uint32_t canary_end;
#endif
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

void StackRellocUp(struct Stack *myStack, float koef_capacity, struct StackErrors* stackErrors);
void StackDump(struct Stack* myStack, struct StackErrors* stackErrors, const char *file, int line, const char *function);
struct StackErrors StackOk(struct Stack* myStack);
void StackCtor(struct Stack* myStack, struct StackErrors* stackErrors);
void StackPush(struct Stack* myStack, Elem_t value, struct StackErrors* stackErrors);
void StackDtor(struct Stack* myStack, struct StackErrors* stackErrors);
Elem_t StackPop(struct Stack* myStack, struct StackErrors* stackErrors);
unsigned int CalculateHash (struct Stack* myStack);
void PrintStackErrors(struct StackErrors* stackErrors);
unsigned long long* PointerLeftCanary (struct Stack* myStack);
unsigned long long* PointerRightCanary (struct Stack* myStack);
