#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventario.h"

void allocare(tabInv_t ***tabInv, int n) {
    **tabInv = (tabInv_t *) malloc(sizeof(tabInv_t));
    (**tabInv)->maxInv = n;
    (**tabInv)->nInv = n;
    (**tabInv)->vettInv = (inv_t *) malloc(n * sizeof(inv_t));
}

void ins_ogg(tabInv_t **tabInv, int *flag) {
    //file lo apro qui
    FILE *fp;
    int n;

    fp = fopen("inventario.txt", "r");

    if(fp == NULL)
        return;

    fscanf(fp, "%d\n", &n);
    if(*flag) {
        allocare(&tabInv, n);
        *flag = 0;
    }

    for(int i = 0; i < n; i++) {
        fscanf(fp, "%s %s %d %d %d %d %d %d\n", (*tabInv)->vettInv[i].nome, (*tabInv)->vettInv[i].tipo,
               (*tabInv)->vettInv[i].stat, ((*tabInv)->vettInv[i].stat) + 1, ((*tabInv)->vettInv[i].stat) + 2,
               ((*tabInv)->vettInv[i].stat) + 3, ((*tabInv)->vettInv[i].stat) + 4, ((*tabInv)->vettInv[i].stat) + 5);
    }

    fclose(fp);
}

int search_ogg(tabInv_t *tabInv, char *name) { //funzione restituisce -1 se non trova, numero indice se trova
    int i;

    for(i = 0; i < tabInv->nInv && strcmp((tabInv->vettInv + i)->nome,name) != 0; i++);
    return i == tabInv->nInv? -1:i;
}

void stampa_ogg(tabInv_t *tabInv, char *name, int width) {
    int index;
    index = search_ogg(tabInv, name);
    if(index == -1)
        printf("Nessun oggetto con quel nome trovato!\n");
    else
        printf("%*cNome: %s\n"
           "%*cTipo: %s\n"
           "%*cStatistiche: %d %d %d %d %d %d\n",
           width, ' ', (tabInv->vettInv + index)->nome, width, ' ', (tabInv->vettInv + index)->tipo, width, ' ',
           (tabInv->vettInv + index)->stat[0], (tabInv->vettInv + index)->stat[1], (tabInv->vettInv + index)->stat[2],
           (tabInv->vettInv + index)->stat[3], (tabInv->vettInv + index)->stat[4], (tabInv->vettInv + index)->stat[5]);
}

void deall_inv(tabInv_t *tabInv, int *flag) {
    if(!*flag) {
        free(tabInv->vettInv);
        free(tabInv);
    }
}