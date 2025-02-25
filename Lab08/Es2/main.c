#include <stdio.h>
#include <stdlib.h>
#include "GRAPH.h"
#include "ST.h"

#define N_SCELTE 4

void riempi_graph(GRAPH *G, FILE *fp);
enum {falso, vero};
typedef int bool;

void stampaMenu(char *scelte[], int *selezione){
    int i = 0;

    printf("\nMENU'\n");
    for(i=0;i<N_SCELTE;i++)
        printf("%2d > %s\n",i,scelte[i]);

    scanf(" %d",selezione);
}

int main() {
    FILE *fp = NULL;
    GRAPH G;
    char *scelte[] = {
            "Vertici ordine alfabetico",
            "Vertici adiacenti (inserire id_elab e id_rete senza spazi)",
            "Genera lista di adiacenza",
            "Fine"
    };
    char *scelte2[] = {
            "Lista",
            "Matrice"
    };
    bool fineprogramma = falso;
    int selezione, *v_ad, n, choice, id[3], adj = 0;
    char nodo1[MAXS], nodo2[MAXS], nodo3[MAXS];

    riempi_graph(&G, fp);
    v_ad = malloc(GRAPHnumv(G)*sizeof(int));

    do {
        stampaMenu(scelte, &selezione);
        switch(selezione) {
            case 0:
                //i nodi sono già in ordine alfabetico, quindi basta stamparli per indici crescenti
                for(int i = 0; i < GRAPHnumv(G); i++) {
                    printf("Vertice: %s\n", STsearch_by_index(GRAPHst(G), i));
                    n = GRAPHvad(G, v_ad, i);
                    printf("Vertici adiacenti:\n");
                    for(int j = 0; j < n; j++)
                        printf("  %s\n", STsearch_by_index(GRAPHst(G),v_ad[j]));
                }
                break;
            case 1:
                printf("\nCHOICE:\n");
                for(int i = 0; i < 2; i++) {
                    printf("%2d > %s\n", i, scelte2[i]);
                }
                scanf(" %d", &choice);
                printf("Inserire nomi nodi:\n");
                scanf("%s %s %s", nodo1, nodo2, nodo3);
                fflush(stdin);
                id[0] = STsearch(GRAPHst(G), nodo1);
                id[1] = STsearch(GRAPHst(G), nodo2);
                id[2] = STsearch(GRAPHst(G), nodo3);
                if(choice) {
                    for(int i = 0; i < 2; i++) {
                        for(int j = i + 1; j < 3; j++) {
                            adj += check_ad_matr(G, id[i], id[j]);
                        }
                    }
                }
                else {
                    for(int i = 0; i < 2; i++) {
                        for(int j = i + 1; j < 3; j++) {
                            adj += check_ad_list(G, id[i], id[j]);
                        }
                    }
                }
                if(adj >= 2)
                    printf("I vertici forniti formano un sottografo completo!\n");
                else
                    printf("I vertici forniti non formano un sottografo completo!\n");

                adj = 0;

                break;
            case 2:
                GRAPHladj(G);
                break;
            case 3:
                fineprogramma = vero;
                break;
            default:
                printf("Errore!");
                break;
        }
    }while(!fineprogramma);

    GRAPHfree(G);
    free(v_ad);

    return 0;
}

void riempi_graph(GRAPH *G, FILE *fp) {

    fp = fopen("grafo.txt", "r");

    *G = GRAPHload(fp);

    fclose(fp);
}