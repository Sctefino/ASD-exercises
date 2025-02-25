#include "Graph.h"
#include <stdlib.h>

int main() {
    FILE *fin;
    Graph G, DAG;
    BSOL x;
    Edge *max, *set;
    int *ts, *sources;


    fin = fopen("grafo4.txt", "r");

    G = GRAPHload(fin);

    fclose(fin);

    set = allocare(G);

    x = min_dag(G, set);

    bsolstore(G, x);

    max = max_weight(x);

    DAG = G;

    //creo DAG rimuovendo archi a peso massimo da grafo iniziale
    for(int i = 0; i < card_bsol(x); i++) {
        GRAPHremoveE(DAG, max[i]);
    }

    ts = malloc(GRAPH_vert(DAG) * sizeof(int));
    DAGrts(DAG, ts);

    sources = calloc(GRAPH_vert(DAG), sizeof(int));

    find_sources(DAG, ts, sources);

    for(int i = 0; i < GRAPH_vert(DAG); i++) {
        if(sources[i] == 1)
            GRAPHspD_DAG(DAG, i, ts);
    }

    free(ts);
    free(sources);
    GRAPHfree(DAG);
    deall(set);
    deall(max);
    deall_bsol(x);

    return 0;
}