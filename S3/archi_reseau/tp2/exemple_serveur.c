#include "client_serveur.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <sys/stat.h>

#define DEBUG 0

#define VERIFIER(expr)                                                     \
    if (!(expr)) {                                                         \
        fprintf(stderr, "%s:%d: erreur: %s\n", __FILE__, __LINE__, #expr); \
        exit(2);                                                           \
    }

void gestionnaire_sigchld(int sig)
{
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}

void traiter_client(int sock)
{
    int pid_client;
    int ret;
    char nom_fichier[256];

    ret = read(sock, &pid_client, sizeof(int));
    VERIFIER(ret == sizeof(int));
    printf("# Connexion du client PID=%d\n", pid_client);

    ret = read(sock, nom_fichier, sizeof(nom_fichier));
    VERIFIER(ret > 0);
    if (nom_fichier[ret - 1] != '\0') {
        if (ret < (int)sizeof(nom_fichier))
            nom_fichier[ret] = '\0';
        else
            nom_fichier[sizeof(nom_fichier) - 1] = '\0';
    }
    printf("# [PID %d] Nom du fichier reçu: %s\n", pid_client, nom_fichier);

    FILE *f = fopen(nom_fichier, "rb");
    if (!f) {
        uint32_t sz_net = htonl((uint32_t)-1);
        write(sock, &sz_net, sizeof(sz_net));
        fprintf(stderr, "# Erreur: impossible d'ouvrir le fichier %s\n", nom_fichier);
        close(sock);
        return;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        uint32_t sz_net = htonl((uint32_t)-1);
        write(sock, &sz_net, sizeof(sz_net));
        fclose(f);
        close(sock);
        return;
    }
    long filesize = ftell(f);
    rewind(f);
    if (filesize < 0) filesize = 0;

    uint32_t sz_net = htonl((uint32_t)filesize);
    VERIFIER(write(sock, &sz_net, sizeof(sz_net)) == sizeof(sz_net));

    char buffer[4096];
    size_t tosend = (size_t)filesize;
    while (tosend > 0) {
        size_t r = fread(buffer, 1, sizeof(buffer), f);
        if (r == 0) break;
        size_t off = 0;
        while (off < r) {
            ssize_t w = write(sock, buffer + off, r - off);
            VERIFIER(w > 0);
            off += (size_t)w;
        }
        tosend -= r;
    }
    fclose(f);
    close(sock);
}






int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    signal(SIGCHLD, gestionnaire_sigchld);

    int lsock = creer_serveur_tcp(port, DEBUG);
    VERIFIER(lsock != -1);

    printf("# Serveur en attente de connexions sur le port %d...\n", port);

    while (1) {
        int sock = attendre_client_tcp(lsock, DEBUG);
        VERIFIER(sock != -1);

        printf("# Nouvelle connexion acceptée\n");

        pid_t pid = fork();
        VERIFIER(pid != -1);

        if (pid == 0) {
            close(lsock);
            traiter_client(sock);
            exit(0);
        } else {
            close(sock);
        }
    }

    close(lsock);
    return 0;
}
