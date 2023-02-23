#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char *args[16] = {0};
    int i = 0, m = 1;
    for (; argv[m]; ++i, ++m)
    {
        args[i] = argv[m];
    }
    char c;
    char buf[32][10];
    int j = 0, k = 0;
    while (read(0, &c, 1) != 0)
    {
        if (c != '\n')
        {
            buf[j][k++] = c;
        }
        else
        {
            buf[j][k] = '\0';
            args[i++] = buf[j];
            j++;
            k = 0;
        }
    }
    if (fork() == 0)
    {
        exec(args[0], args);
    }
    else
    {
        wait(0);
    }
    exit(0);
}