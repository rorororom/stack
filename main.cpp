#include <stdlib.h>

//#include "stack_funcs.h"
//#include "stack_funcs_canary.h"
#include "log_funcs.h"
#include "stack_funcs_hash.h"

int main()
{
    OpenLogFile ("LOH");

    #ifdef WITH_CANARY_AND_HASHE
    StackFuncHash ();

    #else
    printf ("hello world\n");
    #endif
}

//-DWITH_CANARY_AND_HASHE

