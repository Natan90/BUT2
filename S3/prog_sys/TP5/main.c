//gcc -g -Og -Wall -Wextra main.c -o tp5
//./main
//.tp5 ls
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char cmd[512];
    cmd[0] = '\0';

    if (argc == 1) {
        strcpy(cmd, "bash");
    } else {
        for (int i = 1; i < argc; i++) {
            strcat(cmd, argv[i]);
            if (i != argc-1) strcat(cmd, " ");
        }
    }

    char *xargv[] = { "xterm", "-hold", "-e", "sh", "-c", cmd, NULL };

    execvp("xterm", xargv);

    fprintf(stderr, "Erreur execvp: %s\n", strerror(errno));
    return 1;
}
