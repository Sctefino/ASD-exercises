#include "Graph.h"
#include "ST.h"
#include <stdlib.h>

typedef struct node *link;

struct node {
    int v;
    int wt;
    link next;
};

struct graph {
    int V;
    int E;
    link *ladj;
    link z;
    ST tab;
};

struct edges {
    Edge **e;
    int card;
    int n;
    int nmax;
};

link NEWnode(int V, int wt, link next) {
    link x;

    x = malloc(sizeof(struct node));
    x->v = V;
    x->wt = wt;
    x->next = next;

    return x;
}

Graph GRAPHinit(int V) {
    Graph G;

    G = malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    G->ladj = malloc(V * sizeof(link));
    G->z = NEWnode(-1,-1,NULL);
    for(int i = 0; i < G->V; i++) {
        G->ladj[i] = G->z;
    }
    G->tab = STinit(V);
    return G;
}

void GRAPHfree(Graph G) {
    link t, next;
    for(int i = 0; i < G->V; i++) {
        for(t = G->ladj[i]; t != G->z; t = next) {
            next = t->next;
            free(t);
        }
    }
    free(G->ladj);
    free(G->z);
    STfree(G->tab);
    free(G);
}

static Edge Edgecreate(int a, int b, int wt) {
    Edge e;
    e.v = a;
    e.w = b;
    e.wt = wt;
    return e;
}

Graph GRAPHload(FILE *fin) {
    Graph G;
    char name[MAXS], name2[MAXS];
    int id1, id2, wt, V;

    fscanf(fin, "%d\n", &V);

    G = GRAPHinit(V);

    for(int i = 0; i < V; i++) {
        fscanf(fin, "%s\n", name);
        STinsert(G->tab, name);
    }

    while(fscanf(fin, "%s %s %d\n", name, name2, &wt) == 3) {
        id1 = STsearch(G->tab, name);
        id2 = STsearch(G->tab, name2);
        if(id1 >= 0 && id2 >= 0)
            GRAPHinsertE(G, Edgecreate(id1, id2, wt));
    }
    return G;
}

void GRAPHinsertE(Graph G, Edge e) {
    G->ladj[e.v] = NEWnode(e.w, e.wt, G->ladj[e.v]);
    G->E++;
}

void GRAPHremoveE(Graph G, Edge e) {
    link t, p;
    if(G->ladj[e.v] == G->z)
        return;
    for(t = G->ladj[e.v], p = NULL; t != G->z; p = t, t = t->next)
        if(t->v == e.w){
            if(t == G->ladj[e.v])
                G->ladj[e.v] = t->next;
            else
                p->next = t->next;
            free(t);
            G->E--;
            return;
        }
}

void GRAPHstore(Graph G, FILE *fout) {
    for(int i = 0; i < G->V; i++) {
        for(link t = G->ladj[i]; t != G->z; t = t->next) {
            fprintf(fout, "%s %s %d\n", STsearch_by_index(G->tab, i), STsearch_by_index(G->tab, t->v), t->wt);
        }
    }
}

void dfsR(Graph G, Edge e, int *time, int *pre, int *post, int *st, int *flag) { //se trovo anche solo un arco back, c'è ciclo

    st[e.w] = e.v;
    pre[e.w] = (*time)++;

    for(link t = G->ladj[e.w]; t != G->z; t = t->next) {
        if(pre[t->v] == -1) {
            dfsR(G, Edgecreate(e.w, t->v, t->wt), time, pre, post, st, flag);
        }
        else {
            if(post[t->v] == -1) {
                *flag = 0;
            }
        }
    }
    post[e.w] = (*time)++;

}

void GRAPHdfs(Graph G, int id, int *flag, Edge *sol, int k) {
    int time = 0, *pre, *post, *st;
    pre = malloc(G->V * sizeof(int));
    post = malloc(G->V * sizeof(int));
    st = malloc(G->V * sizeof(int));

    //rimuovi archi e fai dfs, poi reinseriscili
    for(int i = 0; i < k; i++)
        GRAPHremoveE(G, sol[i]);


    for(int i = 0; i < G->V; i++) {
        pre[i] = -1;
        post[i] = -1;
        st[i] = -1;
    }

    dfsR(G, Edgecreate(id, id, 1), &time, pre, post, st, flag);
    for(int i = 0; i < G->V; i++)
        if(pre[i] == -1)
            dfsR(G, Edgecreate(i, i, 1), &time, pre, post, st, flag);

    for(int i = 0; i < k; i++)
        GRAPHinsertE(G, sol[i]);
}

void comb_semp(Graph G, Edge *set, Edge *sol, BSOL bsol, int k, int n, int *flag, int pos, int start) {
    int flag_nc = 1;
    if(pos >= k) {
        GRAPHdfs(G, 0, &flag_nc, sol, k);
        if(flag_nc) {
            *flag = 1;
            if(k != 0){
                bsol->card = k;
                if (bsol->n >= bsol->nmax) {
                    bsol->nmax = 2 * bsol->nmax;
                    bsol->e = realloc(bsol->e, bsol->nmax * sizeof(Edge *));
                }
                bsol->e[bsol->n] = malloc(bsol->card * sizeof(Edge));
                for (int i = 0; i < k; i++)
                    bsol->e[bsol->n][i] = sol[i];
                bsol->n++;
            }
        }
        return;
    }

    for(int i = start; i < n; i++) {
        sol[pos] = set[i];
        comb_semp(G, set, sol, bsol, k, n, flag, pos+1, i+1);
    }
}

BSOL min_dag(Graph G, Edge *set) {
    Edge sol[G->E];
    int flag = 0, pos = 0, start = 0; //sovrastimo i vettori, sapendo che avranno per la prima combinazione esattamente
                                    // G->E elementi, mentre poi verranno riempiti sempre con un elemento in meno
    BSOL bsol;


    bsol = malloc(sizeof(*bsol));
    bsol->e = malloc(sizeof(Edge *));
    bsol->n = 0;
    bsol->nmax = 1;
    bsol->card = 0;

    GRAPHedges(G, set);

    for(int k = 0; k < G->E; k++) { //soluzione minima è 0 archi (grafo è già dag)
        comb_semp(G, set, sol, bsol, k, G->E, &flag, pos, start);
        if(flag)
            break;
    }


    return bsol;
}

void bsolstore(Graph G, BSOL x) {

    if(x->n == 0) {
        printf("Il grafo fornito e' gia' un DAG!");
        return;
    }
    for(int i = 0; i < x->n; i++) {
        for(int j = 0; j < x->card; j++) {
            printf("%s %s %d ", STsearch_by_index(G->tab, x->e[i][j].v), STsearch_by_index(G->tab, x->e[i][j].w), x->e[i][j].wt);
        }
        printf("\n");
    }
}

Edge *max_weight(BSOL x) {
    Edge *max_e;
    int max = 0, bmax[2] = {0, -1};

    max_e = malloc(x->card * sizeof(Edge));

    for(int i = 0; i < x->n; i++) {
        for(int j = 0; j < x->card; j++)
            max += x->e[i][j].wt;

        if(max > bmax[0]) {
            bmax[0] = max;
            bmax[1] = i;
        }
        max = 0;
    }

    for(int i = 0; i < x->card; i++) {
        max_e[i] = x->e[bmax[1]][i];
    }

    return max_e;
}

int card_bsol(BSOL x) {
    return x->card;
}

Edge *allocare(Graph G) {
    Edge *x = malloc(G->E * sizeof(Edge));
    return x;
}

int GRAPH_vert(Graph G) {
    return G->V;
}

void GRAPHedges(Graph G, Edge *a) {
    int n = 0;
    for(int i = 0; i < G->V; i++)
        for(link t = G->ladj[i]; t != G->z; t = t->next)
            a[n++] = Edgecreate(i, t->v, t->wt);
}

void TSdfsR(Graph DAG, int v, int *ts, int *pre, int *time) {
    link t;
    int w;

    pre[v] = 0;
    for(w = 0; w < DAG->V; w++) {
        for (t = DAG->ladj[w]; t != DAG->z; t = t->next)
            if (pre[w] == -1 && t->v == v)
                TSdfsR(DAG, w, ts, pre, time);
    }
    ts[(*time)++] = v;
}

void DAGrts(Graph DAG, int *ts) {
    int v, time = 0, pre[DAG->V];

    for(v = 0; v < DAG->V; v++) {
        pre[v] = -1;
        ts[v] = -1;
    }

    for(v = 0; v < DAG->V; v++)
        if(pre[v] == -1)
            TSdfsR(DAG, v, ts, pre, &time);

}

void dfsR_pre(Graph G, Edge e, int *pre) {
    pre[e.w] = 1;

    for(link t = G->ladj[e.w]; t != G->z; t = t->next) {
        if(pre[t->v] == -1) {
            dfsR_pre(G, Edgecreate(e.w, t->v, t->wt), pre);
        }
    }

}
void find_sources(Graph DAG, int *ts, int *sources) {
    int pre[DAG->V];

    for(int i = 0; i < DAG->V; i++) {
        pre[i] = -1;
    }

    for(int i = 0; i < DAG->V; i++) {
        if(pre[ts[i]] == -1) {
            sources[ts[i]] = 1;
            dfsR_pre(DAG, Edgecreate(ts[i], ts[i], 1), pre);
        }
    }
}

void GRAPHspD_DAG(Graph DAG, int id, int *ts) {
    int *st, *d, start, v;

    st = malloc(DAG->V*sizeof(int));
    d = malloc(DAG->V*sizeof(int));

    for(v = 0; v < DAG->V; v++) {
        st[v] = -1;
        d[v] = INT_MIN;
    }

    //considero solo i vertici dopo la sorgente nell'ordinamento topologico: dato che gli archi possono andare solo da sx verso
    //dx per definizione, i vertici a sinistra di una sorgente centrale nel vettore ts non avranno un cammino che va dalla sorgente
    //ad essi, quindi saranno di certo a peso infinito
    for(start = 0; ts[start] != id; start++);

    d[id] = 0;
    st[id] = id;

    for(int i = start; i < DAG->V; i++) {
        if(d[v = ts[i]] != INT_MIN)
            for(link t = DAG->ladj[v]; t != DAG->z; t = t->next)
                if(d[v] + t->wt > d[t->v]) {
                    d[t->v] = d[v] + t->wt;
                    st[t->v] = v;
                }
    }

    printf("\nMax dist from %s\n", STsearch_by_index(DAG->tab, id));

    for(v = 0; v < DAG->V; v++)
        printf("%s: %d\n", STsearch_by_index(DAG->tab, v), d[v] == INT_MIN? INT_MAX : d[v]);
}

void deall(Edge *a) {
    free(a);
}

void deall_bsol(BSOL x) {
    for(int i = 0; i < x->n; i++) {
        free(x->e[i]);
    }
    free(x->e);
    free(x);
}