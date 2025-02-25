#include "invArray.h"

#define MAXINV 50

 struct invArray_s {
    inv_t *vettInv;
    int nInv,
        maxInv;
};

invArray_t invArray_init() {
    invArray_t tabInv = malloc(sizeof(struct invArray_s));
    tabInv -> vettInv = malloc(MAXINV * sizeof(inv_t));
    tabInv -> nInv = 0;
    tabInv -> maxInv = MAXINV;
    return tabInv;
}

void invArray_free(invArray_t invArray) {
    free(invArray->vettInv);
    free(invArray);
}

void invArray_read(FILE *fp, invArray_t invArray) {
    static int n;

    fscanf(fp, "%d\n", &n);
    if(n > MAXINV) {
        invArray->vettInv = realloc(invArray->vettInv, n * sizeof(inv_t));
    }
    for(int i = 0; i < n; i++) {
        inv_read(fp, invArray->vettInv + i);
    }

    invArray -> nInv = n;
}

void invArray_print(FILE *fp, invArray_t invArray) {

    for(int i = 0; i < invArray -> nInv; i++) {
        inv_print(fp, invArray->vettInv + i);
    }
}

void invArray_printByIndex(FILE *fp, invArray_t invArray, int index) {
    inv_print(fp, invArray->vettInv + index);
}

inv_t *invArray_getByIndex(invArray_t invArray, int index) {
    return invArray->vettInv + index;
}

int invArray_searchByName(invArray_t invArray, char *name) {
    for(int i = 0; i < invArray->nInv; i++) {
        if(strcmp(invArray->vettInv[i].nome, name) == 0)
            return i;
    }

    return -1;
}