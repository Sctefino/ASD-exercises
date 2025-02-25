#ifndef I_H
#define I_H

typedef struct {
    char nome[51],
    tipo[51];
    int stat[6];
} inv_t;

typedef struct {
    inv_t *vettInv;
    int nInv,
    maxInv;
} tabInv_t;

void ins_ogg(tabInv_t **tabInv, int *flag);
int search_ogg(tabInv_t *tabInv, char *name);
void stampa_ogg(tabInv_t *tabInv, char *name, int width); //mi porto il width solo per format di stampa per oggetti equip
void deall_inv(tabInv_t *tabInv, int *flag);

#endif