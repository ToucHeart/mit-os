#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

void child(int *p)
{
    int x;
    if (read(p[PIPE_READ], &x, sizeof(x)) == 0)
    {
        exit(0);
    }
    printf("prime %d\n", x);
    int remain;
    int q[2];
    pipe(q);
    close(p[PIPE_WRITE]);
    if (fork() == 0)
    {
        child(q);
    }
    else
    {
        close(q[PIPE_READ]);
        while (read(p[PIPE_READ], &remain, sizeof(remain)) == sizeof(remain))
        {
            if (remain % x != 0)
            {
                write(q[PIPE_WRITE], &remain, sizeof(remain));
            }
        }
        close(p[PIPE_READ]);
        close(q[PIPE_WRITE]);
        wait(0);
        exit(0);
    }
}
int main()
{
    int p[2];
    pipe(p);
    if (fork() == 0)
    {
        child(p);
    }
    else
    {
        close(p[PIPE_READ]);
        for (int i = 2; i <= 35; ++i)
        {
            write(p[PIPE_WRITE], &i, 4);
        }
        close(p[PIPE_WRITE]);
        wait(0);
    }
    exit(0);
}
