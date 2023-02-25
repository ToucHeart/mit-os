#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void f(int *p)
{
    close(p[1]);
    int pid = fork();
    if (pid == 0)
    {
        int prime;
        if (read(p[0], &prime, 4) == 0)
        {
            exit(0);
        }
        printf("prime %d\n", prime);
        int q[2];
        pipe(q);
        int remain;
        while (read(p[0], &remain, 4) != 0)
        {
            if (remain % prime != 0)
            {
                write(q[1], &remain, 4);
            }
        }
        close(p[0]);
        f(q);
    }
    else if (pid > 0)
    {
        close(p[0]);
        wait(0);
        return;
    }
    else
    {
        printf("fork failed\n");
        exit(1);
    }
}
int main()
{
    int p[2];
    pipe(p);
    for (int i = 2; i <= 35; ++i)
    {
        write(p[1], &i, 4);
    }
    f(p);
    exit(0);
}
