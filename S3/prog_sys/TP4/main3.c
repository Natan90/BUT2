#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {

    pid_t pid;
    for (int i = 0; i < 100; i++) {
        pid = fork();
        if (pid > 0) {
            // père attend son fils et s'arrête
            wait(NULL);
            break;
        } else if (pid == 0) {
            printf("Generation %d : je suis le processus %d, père %d\n", i, getpid(), getppid());
            if (i == 99) {
                printf("Dernier descendant terminé !\n");
            }
        } else {
            perror("fork");
            exit(1);
        }
    }
}
