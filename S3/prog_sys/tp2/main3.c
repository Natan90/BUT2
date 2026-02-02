#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copie3(FILE *entree, FILE *sortie) {
    int count = 0;
    char ch;

    if (entree == NULL) {
        printf("Error in opening file\n");
    }


    while ((ch = getc(entree)) != EOF) {
        putc(ch, sortie);
        count++;
    }
    printf("%d\n", count);


    return 0;
}



int main3(void) {
    FILE* FEntree = fopen("../main.c", "r");
    FILE* FSortie = fopen("sortie.c", "w");

    copie3(FEntree, FSortie);
}