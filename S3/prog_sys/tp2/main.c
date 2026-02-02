#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copie(FILE *FBin, FILE *FAscii, FILE *FHex, FILE *FTest) {
    int count = 0;
    char ch;

    int tailleTab = 60;
    int tab[tailleTab];

    for (int i = 0; i<tailleTab; i ++) {
        tab[i] = i+1;
        ch = tab[i];
        printf("%02d ", tab[i]);

        putc(ch, FBin);
        putc(ch, FAscii);
        putc(ch, FHex);
        putc(ch, FTest);


    }





    // while ((ch = getc(entree)) != EOF) {
    //     putc(ch, sortie);
    //     count++;
    // }
    // printf("%d\n", count);


    return 0;
}
int main(void) {
    FILE* FBin = fopen("res.bin", "w");
    FILE* FAscii = fopen("res.ascii", "w");

    FILE* FHex = fopen("res.hex", "w");

    FILE* FTest = fopen("test.txt", "w");


    copie(FBin, FAscii, FHex, FTest);

    return 0;
}