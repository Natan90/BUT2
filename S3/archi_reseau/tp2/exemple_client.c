#include "client_serveur.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdint.h>

#define DEBUG 0

#define VERIFIER(expr)                                                     \
    if (!(expr)) {                                                         \
        fprintf(stderr, "%s:%d: erreur: %s\n", __FILE__, __LINE__, #expr); \
        exit(2);                                                           \
    }

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s serveur port\n", argv[0]);
        return 1;
    }
    char *serveur = argv[1];
    int port = atoi(argv[2]);


    // Réserver un tampon pour lire un nom de fichier en toute sécurité
    char fileName[256];
    int ret;

    int pid = getpid();

    int sock = creer_client_tcp(serveur, port, DEBUG);
    VERIFIER(sock != -1);

    //message de connexion au serveur
    ret = write(sock , &pid, sizeof(int));
    VERIFIER(ret == sizeof(int));

    printf("? ");
    ret = scanf("%255s", fileName);
    VERIFIER(ret == 1);

    ret = write(sock, fileName, (int)strlen(fileName) + 1);
    VERIFIER(ret == (int)strlen(fileName) + 1);
    //fflush(stdout);



    



    uint32_t sz_net;
    ret = read(sock, &sz_net, sizeof(sz_net));
    VERIFIER(ret == sizeof(sz_net));
    uint32_t filesize = ntohl(sz_net);
    if (filesize == (uint32_t)-1) {
        fprintf(stderr, "Erreur du serveur: fichier introuvable ou erreur d'ouverture\n");
        close(sock);
        return 1;
    }

    printf("# Taille du fichier à recevoir: %u octets\n", filesize);

    char outname[300];
    snprintf(outname, sizeof(outname), "recv_%s", fileName);
    FILE *out = fopen(outname, "wb");
    VERIFIER(out != NULL);

    uint32_t remaining = filesize;
    char buffer[4096];
    while (remaining > 0) {
        ssize_t r = read(sock, buffer, remaining < sizeof(buffer) ? remaining : sizeof(buffer));
        VERIFIER(r > 0);
        size_t w = fwrite(buffer, 1, (size_t)r, out);
        VERIFIER(w == (size_t)r);
        remaining -= (uint32_t)r;
    }

    fclose(out);
    close(sock);
    printf("# Fichier reçu et sauvegardé sous '%s'\n", outname);

    return 0;
}
