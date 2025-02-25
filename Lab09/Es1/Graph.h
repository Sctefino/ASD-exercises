#ifndef ES1_GRAPH_H
#define ES1_GRAPH_H
#include <stdio.h>

typedef struct edge {
    int v;
    int w;
    int wt;
} Edge;

typedef struct edges *BSOL;

typedef struct graph *Graph;

Graph GRAPHinit(int V);
void GRAPHfree(Graph G);
Graph GRAPHload(FILE *fin);
void GRAPHstore(Graph G, FILE *fout);
void GRAPHinsertE(Graph G, Edge e);
void GRAPHremoveE(Graph G, Edge e);
void GRAPHedges(Graph G, Edge *a);
void GRAPHdfs(Graph G, int id, int *flag, Edge *sol, int k);
BSOL min_dag(Graph G, Edge *set);
Edge *allocare(Graph G);
void deall(Edge *a);
void bsolstore(Graph G, BSOL bsol);
Edge *max_weight(BSOL x);
int card_bsol(BSOL x);
int GRAPH_vert(Graph G);
void DAGrts(Graph DAG, int *ts);
void find_sources(Graph DAG, int *ts, int *sources);
void GRAPHspD_DAG(Graph G, int id, int *ts);
void deall_bsol(BSOL x);

#endif //ES1_GRAPH_H
