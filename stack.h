#ifndef STACK
#define STACK

typedef double Elem_t;
typedef unsigned long long Canary_t;

const int SIZE = 10;
const int CAPACITY = SIZE + 5;
const int HASH_CONST = 234;
const int XOR_CONST = 555555555;
const int MOD_FOR_HASH = 107;
const Elem_t POISONELEMENT = -1;
const int DOP_RIGHT = 1;
const int DOP_LEFT = 1;
const float UP_KOEF = 2.0;
const float DOWN_KOEF = 0.5;

const Canary_t BUF_CANARY =  0xDEDEEDED;

#define LINE __LINE__
#define FILE __FILE__

#define STACK_DUMP(myStack) StackDump(myStack, FILE, LINE, __func__);

#define STACK_VERIFY(myStack) {        \
    if (StackVerify(myStack) > 0) {    \
        STACK_DUMP(myStack);           \
    }                                  \
}

#define CALCULATE_HASH(stack) stack->hash = CalculateHash(stack)

/**
 * @brief               Elements needed to work in a stack
 * @param canary_start  canary at the beginning of the structure
 * @param data          data array
 * @param capacity      stack volume
 * @param size          stack size
 * @param hash          stack hash
 * @param canary_end    canary at the ending of the structure
 */
struct Stack {
#ifdef WITH_CANARY
    uint32_t canary_start;
#endif

    Elem_t* data;
    int capacity;
    int size;

#ifdef WITH_HASH
    long long hash;
#endif

#ifdef WITH_CANARY
    uint32_t canary_end;
#endif
};

enum StackErrors {
    ERROR_SIZE_BIT          = 1,
    ERROR_CAPACITY_BIT      = 1 << 2,
    ERROR_DATA_BIT          = 1 << 3,
    ERROR_PUSH_BIT          = 1 << 4,
    ERROR_HASH_BIT          = 1 << 5,
    ERROR_CANARY_START_BIT  = 1 << 6,
    ERROR_CANARY_END_BIT    = 1 << 7,
    ERROR_DATA_END_BIT      = 1 << 8,
    ERROR_DATA_START_BIT    = 1 << 9
};

//
//! @brief creator of all data on the stack
//! @param [in] myStack all structure data
//!
//!
//! sets initial parameters for the structure and allocates memory for the initial stack size
void StackCtor(struct Stack* myStack);

//
//! @brief destroys all stack data
//! @param [in] myStack all structure data
//!
//!
//! resets all data and stack pointers
void StackDtor(struct Stack* myStack);

//
//! @brief adding elements to the stack
//! @param [in] myStack all structure data
//!
//!
//! adds elements to the stack and increases the stack size
void StackPush(struct Stack* myStack, Elem_t value);

//
//! @brief deletes elements to the stack
//! @param [in]  myStack all structure data
//! @param [out] ans stack element that is removed and returned
//!
//!
//! returns and removes elements from the stack
Elem_t StackPop(struct Stack* myStack);

//
//! @brief prints all stack data
//! @param [in]  myStack all structure data
//! @param [in]  file the file from which it was called
//! @param [in]  line the line from which it was called
//! @param [in]  function the function from which it was called
//!
//!
//! makes a complete printout of the stack and its data
void StackDump(struct Stack* myStack, const char *file, int line, const char *function);

//! @brief prints all errors
//! @param [in]  sum_errors sum of errors
//!
//!
//! prints all stack related errors
void PrintStackErrors(int sum_errors);

//! @brief calculates hash
//! @param [in]  myStack all structure data
//!
//!
//! recalculates the hash after changes
long long CalculateHash (struct Stack* myStack);

//! @brief the pointer left canary
//! @param [in]  myStack all structure data
//!
//!
//! the pointer changes to the left canary pointer
Elem_t * PointerLeftCanary(struct Stack* myStack);
//! @brief the pointer right canary
//! @param [in]  myStack all structure data
//!
//!
//! the pointer changes to the right canary pointer
Elem_t * PointerRightCanary(struct Stack* myStack);

#endif
