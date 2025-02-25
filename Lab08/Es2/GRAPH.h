#ifndef ES2_GRAPH_H
#define ES2_GRAPH_H
#include <stdio.h>
#include "ST.h"

typedef struct graph *GRAPH;

typedef struct {
    int v;
    int w;
    int wt;
}Edge;

GRAPH GRAPHinit();
void GRAPHfree(GRAPH G);
GRAPH GRAPHload(FILE *fin);
void GRAPHinsertE(GRAPH G, int v, int w, int wt);
int GRAPHnumv(GRAPH G);
ST GRAPHst(GRAPH G);
int GRAPHvad(GRAPH G, int *vad, int index); //riempie un vettore con indici di nodi adiacenti a quello considerato, e ritorna il numero di vertici trovati
void GRAPHladj(GRAPH G);
int check_ad_matr(GRAPH G, int n1, int n2);
int check_ad_list(GRAPH G, int n1, int n2);
#endif //ES2_GRAPH_H
