#include "kernel/types.h"
#include "user/user.h"

void primes(int lp[2])
{
    close(lp[1]);
    int first;
    if (read(lp[0], &first, sizeof(int)) == sizeof(int)) {
        printf("prime %d\n", first);
        int p[2];
        pipe(p);

        int data;
        while (read(lp[0], &data, sizeof(int)) == sizeof(int)) {
            if (data % first != 0)
                write(p[1], &data, sizeof(int));
        }
        close(lp[0]);
        close(p[1]);

        if(fork() == 0) {
            primes(p);
        } else {
            close(p[1]);
            wait(0);
        }
    }
    exit(0);

}

int main()
{
    int p[2];
    pipe(p);

    for (int i = 2; i <= 35; i ++) 
        write(p[1], &i, sizeof(int));
    if (fork() == 0) {
        primes(p);
    }else{
        close(p[1]);
        close(p[0]);
        wait(0);
    }
    exit(0);
}