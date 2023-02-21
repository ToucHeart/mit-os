#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int pipefd[2];
    pipe(pipefd);
    int id = fork();
    if (id == 0)
    {
        char data;
        if (read(pipefd[0], &data, 1) < 0)
        {
            printf("read failed from parent\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        close(pipefd[0]);
        data = 'c';
        if (write(pipefd[1], &data, 1) < 0)
        {
            printf("write failed to parent\n");
            exit(1);
        }
        close(pipefd[1]);
        exit(0);
    }
    else if (id > 0)
    {
        char data = 'p';
        if (write(pipefd[1], &data, 1) < 0)
        {
            printf("write failed to child\n");
            exit(1);
        }
        close(pipefd[1]);
        wait(0);
        if (read(pipefd[0], &data, 1) < 0)
        {
            printf("read failed from child\n");
            exit(1);
        }
        close(pipefd[0]);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
    else
    {
        printf("fork failed\n");
        exit(1);
    }
}