#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) // open directory
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
#if 0
    printf("open success\n");
#endif
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if (st.type == T_FILE) // check is a DIR
    {
        printf("%s is not a directory\n", path);
        close(fd);
        return;
    }
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("%s path is too long\n", path);
        close(fd);
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
#if 0
        printf("current file name in path is %s\n", de.name);
#endif
        if (de.inum == 0)
            continue;
        if (strcmp(de.name, filename) == 0)
        {
            printf("%s/%s\n", path, filename);
        }
        memmove(p, de.name, DIRSIZ);
        p[strlen(de.name)] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_DIR)
        {
            if ((strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0))
                continue;
            find(buf, filename);
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc == 3)
    {
        find(argv[1], argv[2]);
    }
    else
    {
        fprintf(2, "arguments not correct\n");
        exit(1);
    }
    exit(0);
}
