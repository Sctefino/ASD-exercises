#include "GRAPH.h"
#include <stdlib.h>
#include <string.h>

typedef struct node *link;

struct node {
    int v;
    int wt;
    link next;
};
struct graph {
    int V;
    int E;
    int **madj;
    link *ladj;
    link z;
    ST tab;
};

static link NEWnode(int v, int wt, link next) {
    link h;
    h = malloc(sizeof(struct node));
    h->v = v;
    h->wt = wt;
    h->next = next;

    return h;
}

GRAPH GRAPHinit() {
    GRAPH G;

    G = malloc(sizeof(*G));
    G->V = 0;
    G->E = 0;
    G->ladj = NULL;
    G->madj = NULL;
    G->z = NEWnode(-1, -1, NULL);
    G->tab = STinit();
    return G;
}

void GRAPHfree(GRAPH G) {
    link t, next;
    for(int i = 0; i < G->V; i++) {
        free(G->madj[i]);
        if(G->ladj != NULL)
            for(t = G->ladj[i]; t != G->z; t = next) {
                next = t->next;
                free(t);
            }
    }
    free(G->madj);
    if(G->ladj != NULL) {
        free(G->ladj);
        free(G->z);
    }
    STfree(G->tab);
    free(G);
}

static void Madj(GRAPH  G, int V) {
    G->madj = malloc(V*sizeof(int *));
    for(int i = 0; i < V; i++) {
        G->madj[i] = calloc(V, sizeof(int));
    }
}



GRAPH GRAPHload(FILE *fin) {
    GRAPH G;
    char name[MAXS], net[MAXS];
    int id1, id2, wt;

    G = GRAPHinit();

    while(!feof(fin)) {
        G->E++; //per assunzione non si tratta di un multigrafo, quindi non ci sono archi doppioni
        fscanf(fin, "%s %s ", name, net);
        strcat(name, net);
        STinsert(G->tab, name);
        fscanf(fin, "%s %s %*d\n", name, net);
        strcat(name, net);
        STinsert(G->tab, name);
    }

    G->V = STsize(G->tab);
    Madj(G, G->V);

    rewind(fin);

    for(int i = 0; i < G->E; i++) {
        //riutilizzo name e net per salvare le informazioni dei vertici degli archi
        fscanf(fin, "%s %s ", name, net);
        strcat(name, net);
        id1 = STsearch(G->tab, name);

        fscanf(fin, "%s %s %d\n", name, net, &wt);
        strcat(name, net);
        id2 = STsearch(G->tab, name);

        if(id1 >= 0 && id2 >= 0)
            GRAPHinsertE(G, id1, id2, wt);
    }

    return G;
}

static Edge Edgecreate(int v, int w, int wt) {
    Edge e;
    e.v = v;
    e.w = w;
    e.wt = wt;
    return e;
}

static void insertE(GRAPH G, Edge E) {
    G->madj[E.v][E.w] = E.wt;
    G->madj[E.w][E.v] = E.wt; //grafo non orientato
}

void GRAPHinsertE(GRAPH G, int v, int w, int wt) {
    insertE(G, Edgecreate(v, w, wt));
}

int GRAPHnumv(GRAPH G) {
    return G->V;
}

ST GRAPHst(GRAPH G) {
    return G->tab;
}

int GRAPHvad(GRAPH G, int *vad, int index) {
    int count = 0;

    for(int i = 0; i < G->V; i++) {
        if(G->madj[index][i] != 0) {
            vad[count++] = i;
        }
    }

    return count;
}

void GRAPHladj(GRAPH G) {
    if(G->ladj == NULL) {
        G->ladj = malloc(G->V*sizeof(link));
    }
    for(int i = 0; i < G->V; i++) {
        G->ladj[i] = G->z;
    }

    for(int i = 0; i < G->V; i++) {
        for(int j = 0; j < G->V; j++) {
            if(G->madj[i][j] != 0) {
                (G->ladj)[i] = NEWnode(i, G->madj[i][j], G->ladj[i]);
            }
        }
    }

}

int check_ad_matr(GRAPH G, int n1, int n2) {
    return G->madj[n1][n2] != 0;
}

int check_ad_list(GRAPH G, int n1, int n2) {
    for(link t = G->ladj[n1]; t != G->z; t = t->next) {
        if(t->v == n2)
            return 1;
    }
    return 0;
}