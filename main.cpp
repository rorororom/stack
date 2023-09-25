#include <stdlib.h>

#include "stack_funcs.h"
#include "log_funcs.h"
#include "stack_funcs_hash.h"

int main()
{
    OpenLogFile ("LOGE");

    #ifdef WITH_CANARY_AND_HASHE
    StackFuncHash ();
    #else
    StackFunc ();
    #endif

    return 0;
}

//-DWITH_CANARY_AND_HASHE

