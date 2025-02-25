#include <stdlib.h>
#include <string.h>
#include "titolo.h"
#include "coll_quotazioni.h"
#include "ora.h"


typedef struct transaction {
    data date;
    ora hour;
    int val;
    int num;
} transazione;

struct title {
    char nome[MAXC];
    int n;
    transazione *transazioni;
    BSTq quot_g;
};

titolo titinit(int n) {
    titolo t;

    t = malloc(sizeof(*t));
    t->n = n;
    t->transazioni = malloc(n * sizeof(transazione));
    t->quot_g = bstinit();

    return t;
}

void freetitle(titolo A) {
    free(A->transazioni);
    BSTfree(A->quot_g);
    free(A);
}

int titcmp(titolo A, titolo B) {
    if(strcmp(A->nome, B->nome) < 0)
        return -1;
    if(strcmp(A->nome, B->nome) > 0)
        return 1;
    return 0;
}

titolo read_title(FILE *fin) {
    char name[MAXC];
    int n, a, mes, g, o, min;
    titolo x;

    fscanf(fin, "%s %d\n", name, &n);

    x = titinit(n);
    strcpy(x->nome, name);
    for(int i = 0; i < n; i++) {
        fscanf(fin, "%d/%d/%d %d:%d %d %d\n", &a, &mes, &g, &o, &min, &(x->transazioni[i].val), &(x->transazioni[i].num));
        x->transazioni[i].date = getdata(a, mes, g);
        x->transazioni[i].hour = getora(o, min);
        BSTinsert(x->quot_g, x->transazioni[i].date, x->transazioni[i].num, x->transazioni[i].val);
    }
    return x;
}

void merge_title(titolo A, titolo B) {


    A->transazioni = realloc(A->transazioni, (A->n + B->n) * sizeof(transazione));
    for(int i = 0; i < B->n; i++) {
        BSTinsert(A->quot_g, B->transazioni[i].date, B->transazioni[i].num, B->transazioni[i].val);
        A->transazioni[A->n + i].date = B->transazioni[i].date;
        A->transazioni[A->n + i].hour = B->transazioni[i].hour;
        A->transazioni[A->n + i].val = B->transazioni[i].val;
        A->transazioni[A->n + i].num = B->transazioni[i].num;
    }
    A->n = A->n + B->n;
    freetitle(B);

}

char *getname(titolo A) {
    return A->nome;
}

void titstore(titolo A) {
    printf("%s\n", A->nome);
    for(int i = 0; i < A->n; i++) {
        datastore(A->transazioni[i].date);
        orastore(A->transazioni[i].hour);
        printf("%d %d\n", A->transazioni[i].val, A->transazioni[i].num);
    }
}

void find_quot(titolo t, int a, int m, int g) {
    BSTsearch(t->quot_g, getdata(a, m, g));
}

void find_quot_date(titolo t, int a, int m, int g, int a1, int m1, int g1) {
    BSTsearch_int(t->quot_g, getdata(a, m, g), getdata(a1, m1, g1));
}

void balance(titolo t, int S) {
    int min = INT_MAX, max = INT_MIN;

    count_path(t->quot_g, &max, &min);
    if((float)max/(float)min > (float)S) { //per evitare che con troncamento 2,3 sia uguale a 2
        BSTbalance(t->quot_g);
        printf("BST correttamente bilanciato!\n");
    }
    else
        printf("Soglia non superata!\n");
}