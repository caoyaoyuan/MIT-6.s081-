#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    int pipefd[2];
    pipe(pipefd);
    int nowpid;
    char buf[2];
    
    if (fork() == 0) {
        nowpid = getpid();
        if (read(pipefd[0], buf, 1) != 1){
            fprintf(2, "failed to read in child\n");
            exit(1);
        }
        close(pipefd[0]);
        printf("%d: received ping\n", nowpid);
        if (write(pipefd[1], buf, 1) != 1) {
            fprintf(2, "failed to write in child\n");
            exit(1);
        }
        close(pipefd[1]);
        exit(0);
    } else{
        nowpid = getpid();
        char info[2] = "a";
        if (write(pipefd[1], info, 1) != 1) {
            fprintf(2, "failed to write in parent\n");
            exit(1);
        }
        close(pipefd[1]);
        wait(0);
        if (read(pipefd[0], buf, 1) != 1){
            fprintf(2, "failed to read in parent\n");
            exit(1);
        }
        close(pipefd[0]);
        printf("%d: received pong\n", nowpid);
        exit(0);
    }
    exit (0);
}
