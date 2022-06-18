#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

int readline(char *new_argv[MAXARG], int cur_argc) {
    char buf[64];
    int cnt = 0;
    while (read(0, buf + cnt, 1)) {
        if (cnt == 64)
        {
            fprintf(2, "arguments is too long\n");
            exit(1);
        }
        if (buf[cnt] == '\n') {
            break;
        }
        cnt++;
    }
    buf[cnt] = 0;
    if (cnt == 0) return 0;
    int offset = 0;
    while (offset < cnt) {
        new_argv[cur_argc++] = buf + offset;
        //new_argv[cur_argc] = malloc(strlen(buf + offset) + 1);
        //strcpy(new_argv[cur_argc++], buf + offset);
        while (buf[offset] != ' ' && offset < cnt)
        {
            offset++;
        }
        while (buf[offset] == ' ' && offset < cnt) 
        {
            buf[offset++] = 0;
        }
    }
    return cur_argc;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(2, "Usage: xargs command arg1 arg2 ...\n");
        exit(1);
    }

    char* command = malloc(strlen(argv[1]) + 1);
    char *new_argv[MAXARG];
    strcpy(command, argv[1]);
    for (int i = 1; i < argc; i ++) {
        new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
        strcpy(new_argv[i - 1], argv[i]);
    }

    int cur_argc;
    while ((cur_argc = readline(new_argv, argc - 1)) != 0) {
        new_argv[cur_argc] = 0;
        if (fork() == 0) {
            exec(command, new_argv);
            fprintf(2, "exec failed\n");
            exit(1);
        }
        wait(0);
    }
    exit(0);
}