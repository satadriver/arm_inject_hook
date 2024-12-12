#include <stdio.h>

#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "And64InlineHook.h"

typedef int (*ptr_old_puts)(const char *);

ptr_old_puts old_puts = 0;

int new_puts(const char *string)
{
    return old_puts("inlineHook 64 bits success");
}




int hook()
{
    A64HookFunction((void*const)puts,(void*const)new_puts,(void **)&old_puts);

    return 0;
}


int InjectEntry(char * param)
{
    puts("test");
    hook();
    puts("test");

    return 0;
}