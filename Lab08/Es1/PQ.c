#include <stdio.h>
#include <stdlib.h>
#include "PQ.h"


//Coda a priorità con vettore ordinato
struct pqueue {
    Item *a;
    int n;
    int nmax;
};

void merge(Item *a, Item *b, int l, int q, int r) {
    int j = l, k = q + 1;

    for(int i = l; i <= r; i++) {
        if(j > q)
            b[i] = a[k++];
        else if(k > r)
            b[i] = a[j++];
        else if(KEYcmp(KEYGet(a[j]), KEYGet(a[k])) == 1)
            b[i] = a[j++];
        else
            b[i] = a[k++];
    }

    for(int i = l; i <= r; i++)
        a[i] = b[i];
}

void mergesortR(Item *a, Item *b, int l, int r) {
    int q;

    if(l >= r)
        return;
    q = (l+r)/2;

    mergesortR(a, b, l, q);
    mergesortR(a, b, q+1, r);

    merge(a, b, l, q, r);
}

void MergeSort(Item *a, int n) {
    Item *b = malloc(n*sizeof(Item));
    int l = 0, r = n - 1;
    mergesortR(a, b, l, r);

    free(b);
}

PQ PQinit(int maxN) {
    PQ pq;

    pq = malloc(sizeof(*pq));
    pq->a = malloc(maxN*sizeof(Item));
    pq->n = 0;
    pq->nmax = maxN;

    return pq;
}

void PQfree(PQ pq) {
    free(pq->a);
    free(pq);
}

int PQempty(PQ pq) {
    return pq->n == 0;
}

int PQsize(PQ pq) {
    return pq->n;
}

Item PQshowMax(PQ pq) {
    return(pq->a[0]);
}

void PQdisplay(PQ pq) {
    for(int i = 0; i < pq->n; i++) {
        ITEMStore(pq->a[i]);
    }
}

void PQinsert(PQ pq, Item val) {
    pq->a[pq->n++] = val;
    MergeSort(pq->a, pq->n);
}

Item el_index(PQ pq, int index) {
    return pq->a[index];
}