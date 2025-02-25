#ifndef I_P
#define I_P
#include "inventario.h"

typedef struct {
    int inUso;
    inv_t **vettEq;
} tabEquip_t;

typedef struct node *link;

typedef struct {
    char codice[7],
    nome[51],
    classe[51];
    tabEquip_t *equip;
    int stat[6];
} pg_t;

typedef struct node{
    pg_t val;
    link next;
} nodoPg_t;

typedef struct {
    link headPg,
    tailPg;
    int nPg;
} tabPg_t;

void ins_pers(FILE *fp, tabPg_t **tabPg, int *flag);
void del_pg(tabPg_t *tabPg, char *code);
link search_pg(tabPg_t *tabPg, char *code);
void stampa_pg(tabPg_t  *tabPg, tabInv_t *tabInv, char *code);
void mod_equipa(tabPg_t *tabPg, tabInv_t *tabInv, char *name, char *code, int choice);
void deall_pg(tabPg_t *tabPg, int *flag);

#endif