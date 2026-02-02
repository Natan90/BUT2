#include "client_serveur.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG 0

#define VERIFIER(expr)                                                     \
    if (!(expr)) {                                                         \
        fprintf(stderr, "%s:%d: erreur: %s\n", __FILE__, __LINE__, #expr); \
        exit(2);                                                           \
    }

int main(void)
{
    int lsock = creer_serveur_tcp(7777, DEBUG);
    VERIFIER(lsock != -1);

    printf("# Serveur itératif (V2 dynamique) sur port 7777\n");

    while (1) {
        int sock = attendre_client_tcp(lsock, DEBUG);
        VERIFIER(sock != -1);

        int size;
        int ret = read(sock, &size, sizeof(int));
        if (ret != sizeof(int)) { close(sock); continue; }

        int *tab = malloc(size * sizeof(int));
        VERIFIER(tab != NULL);

        ret = read(sock, tab, size * sizeof(int));
        VERIFIER(ret == size * sizeof(int));

        int *newTab = malloc(size * sizeof(int));
        VERIFIER(newTab != NULL);

        int newSize = 0;

        for (int i = 0; i < size; i++)
            if (tab[i] != 0)
                newTab[newSize++] = tab[i];

        write(sock, &newSize, sizeof(int));
        write(sock, newTab, newSize * sizeof(int));

        free(tab);
        free(newTab);

        close(sock);
    }
}
