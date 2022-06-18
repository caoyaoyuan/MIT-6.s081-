#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    if (argc <= 1) {
        printf("usage: sleep time\n");
        exit(1);
    }
    int s_time = atoi(argv[1]);
    if (sleep(s_time) == -1){
        printf("error.\n");
        exit(1);
    }
    exit(0);
}