#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#define BT_BUF_SIZE 100

int dump_stack()
{
    int j, nptrs;
    void *buffer[BT_BUF_SIZE];
    char **strings;

    nptrs = backtrace(buffer, BT_BUF_SIZE);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
       return 0;
    }

    for (j = 0; j < nptrs; j++)
    {
       printf("%s\n", strings[j]);
    }

    free(strings);
    return 0;
}

int main()
{
    dump_stack();
    return 0;
}
