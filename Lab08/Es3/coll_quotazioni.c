#include "coll_quotazioni.h"
#include "quotazione.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct BSTnode *link;
struct BSTnode {
    quotazione q;
    link l;
    link r;
    int N;
};
struct binarysearchtreeq {
    link root;
    link z;
};


static link NEWnode (link l, link r, quotazione q, int N){
    link x;
    x = malloc(sizeof(*x));

    x->l = l;
    x->r = r;
    x->q = q;
    x->N = N;

    return x;
}

BSTq bstinit() {
    BSTq x;

    x = malloc(sizeof(*x));
    x->root = x->z = NEWnode(NULL, NULL, quotvoid(), 0);

    return x;
}

static void treeFree(link h, link z) {
    if(h == z)
        return;
    treeFree(h->l, z);
    treeFree(h->r, z);
    free(h);
}

void BSTfree(BSTq bst) {
    if(bst == NULL)
        return;
    treeFree(bst->root, bst->z);
    free(bst->z);
    free(bst);
}

static link insertR(link h, data date, int n, int v, link z, int *flag) {
    if(h == z)
        return NEWnode(z, z, getquot(quotvoid(), date, n, v), 1);
    if(datacmp(quotdate(h->q), date) == 1)
        h->l = insertR(h->l, date, n, v, z, flag);
    else if(datacmp(quotdate(h->q), date) == -1)
        h->r = insertR(h->r, date, n, v, z, flag);
    else {
        h->q = getquot(h->q, date, n, v); //in questo caso non aumento nodi, cambio solo il valore della quotazione in quella data
        *flag = 0;
    }

    if(*flag)
        (h->N)++; //se non sono entrato nell'else, ho aggiunto un nodo, quindi aumento tutti le dimensioni dei nodi soprastanti
    return h;
}

void BSTinsert(BSTq bst, data date, int n, int v) {
    int flag = 1;
    bst->root = insertR(bst->root, date, n, v, bst->z, &flag);
}

static void searchR(link h, data date, link z, int *flag) {
    if(h == z)
        return;
    if(datacmp(quotdate(h->q), date) == 0) {
        *flag = 0;
        printf("%f\n", quotval(h->q));
    }
    if(datacmp(quotdate(h->q), date) == 1)
        searchR(h->l, date, z, flag);
    searchR(h->r, date, z, flag);

}

void BSTsearch(BSTq x, data date) {
    int flag = 1;
    searchR(x->root, date, x->z, &flag);
    if(flag)
        printf("Quotazioni non trovate per quella data!\n");
}

static void searchR_int(link h, data date1, data date2, link z, float *min, float *max) {
    if(h == z)
        return;

    if(datacmp(quotdate(h->q), date1) == 1)
        searchR_int(h->l, date1, date2, z, min, max);

    if(datacmp(quotdate(h->q), date1) == 1 && datacmp(quotdate(h->q), date2) == -1) {
        if (*min > quotval(h->q))
            *min = quotval(h->q);
        if(*max < quotval(h->q))
            *max = quotval(h->q);
    }

    if(datacmp(quotdate(h->q), date2) == -1)
        searchR_int(h->r, date1, date2, z, min, max);
}

void BSTsearch_int(BSTq bst, data date1, data date2) {
    float min = (float)INT_MAX, max = -1; //le quotazioni non possono essere negative

    searchR_int(bst->root, date1, date2, bst->z, &min, &max);
    if(min != (float)INT_MAX && max != -1)
        printf("La quotazione minima nella data selezionata e': %f\n"
               "La quotazione massima nella data selezionata e': %f\n", min, max);
    else
        printf("Quotazioni non trovate per intervallo di date scelto!\n");
}
static void pathR(link h, link z, int *max, int *min, int cnt) {
    if(h == z)
        return;

    pathR(h->l, z, max, min,cnt + 1);
    pathR(h->r, z, max, min,cnt + 1);
    if(cnt > *max) //non serve il controllo di foglia, poiché nessun backtrack ha percorso maggiore di quello per arrivare a foglia
        *max = cnt;
    if(cnt < *min && h->l == z && h->r == z) //solo se è una foglia, non bisogna contare i vari backtrack, sennò minimo sempre = 0
        *min = cnt;
}
void count_path(BSTq bst, int *max, int *min) {
    //visita post order per contare lunghezza cammini e per ordinare chiavi
    int cnt = 0;
    pathR(bst->root, bst->z, max, min, cnt);
}

static link rotL(link h) {
    link x;

    x = h->r;
    h->r = x->l;
    x->l = h;
    x->N = h->N;
    h->N = 1;
    h->N += (h->l)? h->l->N : 0;
    h->N += (h->r)? h->r->N : 0;

    return x;
}
static link rotR(link h) {
    link x;

    x = h->l;
    h->l = x->r;
    x->r = h;
    x->N = h->N;
    h->N = 1;
    h->N += (h->l)? h->l->N : 0;
    h->N += (h->r)? h->r->N : 0;

    return x;
}

static link partR(link h, int r) {
    int t = h->l->N;
    if(t > r) {
        h->l = partR(h->l, r);
        h = rotR(h);
    }
    if(t < r) {
        h->r = partR(h->r, r-t-1);
        h = rotL(h);
    }
    return h;
}

static link balanceR(link h, link z) {
    int r;
    if(h == z)
        return z;
    r = (h->N+1)/2 - 1; //rango mediana
    h = partR(h, r);
    h->l = balanceR(h->l, z);
    h->r = balanceR(h->r, z);
    return h;
}

static void treePrintR(link h, link z) {
    if(h == z)
        return;
    printf("%f ", quotval(h->q));
    treePrintR(h->l, z);
    treePrintR(h->r, z);
}

void BSTbalance(BSTq bst) {
    bst->root = balanceR(bst->root, bst->z);
    //treePrintR(bst->root, bst->z);
}