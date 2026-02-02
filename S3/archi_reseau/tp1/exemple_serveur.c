#include "client_serveur.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#define DEBUG 0

#define VERIFIER(expr)                                                     \
    if (!(expr)) {                                                         \
        fprintf(stderr, "%s:%d: erreur: %s\n", __FILE__, __LINE__, #expr); \
        exit(2);                                                           \
    }

// Gestionnaire de signal pour nettoyer les processus enfants
void gestionnaire_sigchld(int sig)
{
    (void)sig; // Eviter le warning unused parameter
    int saved_errno = errno; // Préserver errno car les gestionnaires peuvent l'altérer
    int status;
    pid_t pid;
    // Nettoyer tous les processus enfants terminés
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // Optionnel: journaliser la fin du fils
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            printf("# [SIGCHLD] Fils %d terminé (code=%d)\n", pid, code);
        } else if (WIFSIGNALED(status)) {
            int sigterm = WTERMSIG(status);
            printf("# [SIGCHLD] Fils %d tué par le signal %d\n", pid, sigterm);
        }
    }
    errno = saved_errno; // Restaurer errno
}

void traiter_client(int sock)
{
    int a, b, c;
    int ret;
    int pid_client;

    ret = read(sock, &pid_client, sizeof(int));
    printf("# Connexion du client avec PID %d\n", pid_client);

    ret = read(sock, &a, sizeof(int));
    VERIFIER(ret == sizeof(int));
    ret = read(sock, &b, sizeof(int));
    VERIFIER(ret == sizeof(int));

    c = a + b;
    printf("# [PID %d] calcul de %d + %d -> %d\n", getpid(), a, b, c);
    ret = write(sock, &c, sizeof(int));
    VERIFIER(ret == sizeof(int));

    ret = close(sock);
    VERIFIER(ret == 0);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);

    // Installer le gestionnaire de signal pour SIGCHLD
    signal(SIGCHLD, gestionnaire_sigchld);

    int lsock = creer_serveur_tcp(port, DEBUG);
    VERIFIER(lsock != -1);

    printf("# Serveur en attente de connexions sur le port %d...\n", port);

    // Boucle infinie pour accepter les connexions
    while (1) {
        int sock;
        // Rendre l'acceptation robuste aux interruptions par signaux (EINTR)
        while (1) {
            sock = attendre_client_tcp(lsock, DEBUG);
            if (sock == -1) {
                if (errno == EINTR) {
                    // accept interrompu par un signal (p.ex. SIGCHLD): réessayer
                    continue;
                }
                VERIFIER(sock != -1);
            }
            break;
        }

        printf("# Nouvelle connexion acceptée\n");

        // Créer un processus enfant pour traiter le client
        pid_t pid = fork();
        VERIFIER(pid != -1);

        if (pid == 0) {
            // Processus enfant
            close(lsock); // Le socket d'écoute n'est pas nécessaire ici
            traiter_client(sock);
            exit(0); // Terminer le processus enfant
        } else {
            // Processus parent
            close(sock); // Le socket client n'est pas nécessaire ici
            // Continuer à accepter d'autres connexions
        }
    }

    close(lsock);

    return 0;
}
