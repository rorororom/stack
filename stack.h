#ifndef STACK
#define STACK

#include <cstdint>

/**
 * @brief class type for stack
 *
 *
 * this class stores the data type for working with the stack
 */
typedef double Elem_t;

/**
 * @brief class type for canary
 *
 *
 * this class stores the data type for working with the canary
 */
typedef unsigned long long Canary_t;

/**
 * @brief array size constant
 *
 *
 * this constant denotes the size of the array at the very beginning. size may vary during the program
 */
const int SIZE = 10;

/**
 * @brief array capacity constant
 *
 *
 * this constant denotes the capacity of the array at the very beginning. capacity may vary during the program
 */
const int CAPACITY = SIZE + 5;

/**
 * @brief hash constant
 *
 *
 * multiplication constant in hash formula
 */
const int HASH_CONST = 234;

/**
 * @brief hash constant
 *
 *
 * xor constant in hash formula
 */
const int XOR_CONST = 555555555;

/**
 * @brief hash constant
 *
 *
 * constant for taking the modulus in a hash formula
 */
const int MOD_FOR_HASH = 107;

/**
 * @brief unused elements constant
 *
 *
 * if the stack element is not yet defined, then this value is used as the value of the element
 */
const Elem_t POISONELEMENT = -1;

/**
 * @brief constant realloc
 *
 *
 * denotes how many times the stack size needs to be increased when new memory is allocated
 */
const float UP_KOEF = 2.0;

/**
 * @brief constant realloc
 *
 *
 * denotes how many times the stack size needs to be reduced when allocating new memory
 */
const float DOWN_KOEF = 0.5;

const Canary_t BUF_CANARY =  0xDEDEEDED;

#define LINE __LINE__
#define FILE __FILE__

#define STACK_DUMP(myStack) StackDump(myStack, FILE, LINE, __func__);

                                                            // do field name in Stack struct and put name of variable via # operator in name field in this define
#define STACK_CONSTRUCT(nameStruct) {       \
    struct Stack nameStruct = {};           \
    stack_ctor(&nameStruct);                \
}


                                                            // StackVerify returns bitmask of errors; let's save its result and if there are any errors
                                                            // return from any function like Stack... via this macros; you only need to return saved StackVerify result
#define STACK_VERIFY(myStack) {             \
    if (StackVerify(myStack) > 0) {         \
        STACK_DUMP(myStack);                \
    }                                       \
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
