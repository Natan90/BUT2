/* TP6 - Signaux / Synchronisation
 * Auteur : un étudiant qui galère un peu
 * Compile avec :
 *    gcc -Wall -Wextra -g tp6_etudiant.c -o tp6_etudiant
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define NB_SIGNS (_NSIG-1)

void gere_signal(int sig) {
    printf("Recu signal %d\n", sig);
    fflush(stdout); // sinon ca s'affiche pas tout de suite
}

void test_signaux() {
    int i;
    struct sigaction sa;
    sa.sa_handler = gere_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    for (i = 1; i < _NSIG; i++) {
        if (sigaction(i, &sa, NULL) == -1) {
            // pas grave si ca marche pas pour certains signaux
            // printf("signal %d pas possible\n", i);
        }
    }

    printf("Tous les signaux installés (enfin presque)\n");
    printf("PID = %d\n", getpid());
    while (1) {
        pause(); // on attend un signal
    }
}

pid_t f1, f2, f3;
int compteur = 0;
int MAX = 10;

void handler_pere(int sig) {
    compteur++;
    if (compteur < MAX) {
        kill(f3, SIGUSR1);
    } else {
        printf("Fin du programme\n");
        kill(f1, SIGTERM);
        kill(f2, SIGTERM);
        kill(f3, SIGTERM);
    }
}

void handler_f1(int sig) {
    printf("Montbéliard\n");
    fflush(stdout);
    kill(getppid(), SIGUSR1);
}

void handler_f2(int sig) {
    printf("Belfort ");
    fflush(stdout);
    kill(f1, SIGUSR1);
}

void handler_f3(int sig) {
    printf("IUT ");
    fflush(stdout);
    kill(f2, SIGUSR1);
}

void synchro_processus() {
    struct sigaction sa;

    // création des fils
    f1 = fork();
    if (f1 == 0) {
        // fils1
        sa.sa_handler = handler_f1;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL);
        while (1) pause();
        exit(0);
    }

    f2 = fork();
    if (f2 == 0) {
        sa.sa_handler = handler_f2;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL);
        while (1) pause();
        exit(0);
    }

    f3 = fork();
    if (f3 == 0) {
        sa.sa_handler = handler_f3;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL);
        while (1) pause();
        exit(0);
    }

    // pere
    sa.sa_handler = handler_pere;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    sleep(1); // attendre que tout soit pret
    kill(f3, SIGUSR1);

    while (compteur < MAX) pause();

    // attendre la mort des fils
    wait(NULL);
    wait(NULL);
    wait(NULL);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage : %s [signaux|sync]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "signaux") == 0) {
        test_signaux();
    } else if (strcmp(argv[1], "sync") == 0) {
        synchro_processus();
    } else {
        printf("Argument inconnu\n");
    }

    return 0;
}
