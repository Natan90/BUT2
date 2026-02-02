#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {

    pid_t pid[100];
    for (int i = 0; i<100; i++){

        pid[i] = fork();
        if (pid[i] == 0) {
            perror("Erreur fork");
        }
        else if (pid[i] == 0) {
            // fils
            printf("Fils %d: pid=%d, ppid=%d\n", i, getpid(), getppid());
        }
    }

    return 0;
}