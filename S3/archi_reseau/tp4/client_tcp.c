#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage : %s <adresse> <port>\n", argv[0]);
        return 1;
    }

    char *server = argv[1];
    int port = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 2;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;          
    addr.sin_port = htons(port);
    if (inet_aton(server, &addr.sin_addr) == 0) {
        printf("Adresse invalide : %s\n", server);
        return 2;
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("connect");
        return 3;
    }

    printf("Connecté au serveur !\n");

    char message[4096];

    while (1) {
        printf("Entrez un message : ");
        scanf("%s", message);
        
        write(sock, message, 4096);
    }

    close(sock);

    return 0;
}
