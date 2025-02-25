#include <stdlib.h>
#include "coll_titoli.h"

#define SCELTE1 3
#define SCELTE2 4

int comandoMenu(char *scelte[], int n) {
    int x;

    for(int i = 0; i < n; i++) {
        printf("\n%d > %s\n", i, scelte[i]);
    }
    scanf("%d", &x);

    return x;
}

int main() {
    char *scelte1[] = {"Acquisizione contenuto file","Ricerca titolo", "Fine"}, *scelte2[] = {"Ricerca quotazione data",
                                                                                              "Ricerca quotazione minima e massima intervallo date",
                                                                                              "Ricerca quotazione minima e massima assolute",
                                                                                              "Bilanciamento albero con rapporto percorso masssimo/minimo maggiore di soglia S"};
    int choice, fineprogramma = 0, anno, mese, giorno, anno1, mese1, giorno1, S;
    char filename[30], name[MAXC];
    FILE *fp;
    coll_titoli lista = collinit();
    titolo x;

    do {
        choice = comandoMenu(scelte1, SCELTE1);
        switch (choice) {
            case 0:
                printf("Inserire nome file:\n");
                scanf("%s", filename);
                fflush(stdin);
                fp = fopen(filename, "r");
                file_read(lista, fp);
                fclose(fp);
                break;
            case 1:
                printf("Inserire nome titolo:\n");
                scanf("%s", name);
                fflush(stdin);
                x = searchTitle(lista, name);
                if(x != NULL) {
                    titstore(x);
                    choice = comandoMenu(scelte2, SCELTE2);
                    switch (choice) {
                        case 0:
                            printf("Inserire data (formato aaaa/mm/gg):\n");
                            scanf("%d/%d/%d", &anno, &mese, &giorno);
                            find_quot(x, anno, mese, giorno);
                            break;
                        case 1:
                            printf("Inserire date (formato aaaa/mm/gg):\n");
                            scanf("%d/%d/%d %d/%d/%d", &anno, &mese, &giorno, &anno1, &mese1, &giorno1);
                            find_quot_date(x, anno, mese, giorno, anno1, mese1, giorno1);
                            break;
                        case 2:
                            //inserisco come valori delle date i numeri minimi e massimi, così attraverso per intero l'albero
                            find_quot_date(x, INT_MIN, INT_MIN, INT_MIN, INT_MAX, INT_MAX, INT_MAX);
                            break;
                        case 3:
                            printf("Inserire soglia S:\n");
                            scanf("%d", &S);
                            balance(x, S);
                            break;
                        default:
                            printf("Comando non inserito correttamente!");
                            break;
                    }
                }
                else
                    printf("Titolo non trovato!");
                break;
            case 2:
                fineprogramma = 1;
                freecoll(lista);
                break;
            default:
                printf("Comando non inserito correttamente!");
                break;
        }
    } while(!fineprogramma);

    return 0;
}
