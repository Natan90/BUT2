#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_BACKLOG 16
#define BUFSIZE 4096

void *connection_thread(void *arg) {
    int sock = *((int*)arg);
    char msg[BUFSIZE];
    int n = 1;

    while (n > 0) {
        n = read(sock, msg, BUFSIZE);
        if (n > 0) {
            printf("Message reçu : >>>%.*s<<<\n", n, msg);
        }
    }

    close(sock);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s port\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock < 0) {
        perror("socket");
        return 2;
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(lsock, (struct sockaddr*)&saddr, sizeof(saddr)) != 0) {
        perror("bind");
        return 2;
    }

    if (listen(lsock, SERVER_BACKLOG) != 0) {
        perror("listen");
        return 2;
    }

    pthread_t t;
    while (1) {
        struct sockaddr_in caddr;
        socklen_t len = sizeof(caddr);
        int csock = accept(lsock, (struct sockaddr*)&caddr, &len);
        printf("IP client : %s\nPort client : %d\n",
               inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

        pthread_create(&t, NULL, connection_thread, &csock);
    }

    close(lsock);
    return 0;
}
