#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


void affiche_car(const char *prefixe) {


    printf("\n");
    printf("%s: ", prefixe);
    printf("%d: ", getpid());
    printf("%d: ", getppid());
}

//Non, on ne peut pas prédire à l’avance le numéro du processus père, car il dépend du système d’exploitation au moment de l’exécution
//Quand on met sleep(5) dans le fils, le père affiche son message tout de suite puis se termine, tandis que le fils affiche le sien 5 secondes plus tard et devient orphelin (adopté par le processus 1).

int main2(void) {

    int status;
    pid_t pid_fils[2];

    for (int i = 0; i < 2; i++) {
        pid_fils[i] = fork();

        if (pid_fils[i] < 0) {
            perror("Erreur fork fils");
            exit(1);
        }
        else if (pid_fils[i] == 0) {
            // fils
            affiche_car("Fils");

            pid_t pid_petit_fils = fork();

            if (pid_petit_fils < 0) {
                perror("Erreur fork petit-fils");
                exit(1);
            }
            else if (pid_petit_fils == 0) {
                // petit fils
                affiche_car("Petit-fils");
                exit(1); // code de retour = 1
            }
            else {
                // fils attend petit fils
                wait(&status);
                if (WIFEXITED(status)) {
                    printf("Processus %d : mon fils %d s'est terminé normalement avec code %d\n",
                           getpid(), pid_petit_fils, WEXITSTATUS(status));
                } else {
                    printf("Processus %d : mon fils %d s'est terminé anormalement\n",
                           getpid(), pid_petit_fils);
                }
                exit(0);
            }
        }
    }

    // pere
    affiche_car("Père");

    for (int i = 0; i < 2; i++) {
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Processus %d : mon fils s'est terminé normalement avec code %d\n",
                   getpid(), WEXITSTATUS(status));
        } else {
            printf("Processus %d : mon fils s'est terminé anormalement\n", getpid());
        }
    }

    return 0;
}