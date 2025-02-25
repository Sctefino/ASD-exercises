#include "pgList.h"

typedef struct nodoPg_t *link;

struct nodoPg_t{
    pg_t val;
    link next;
};

struct pgList_s {
    link headPg,
         tailPg;
    int nPg;
};

link newNode(link h, pg_t pg) {
    link x = malloc(sizeof(struct nodoPg_t));
    if(x == NULL) {
        return NULL;
    }
    x->val = pg;
    x->next = h;

    return x;
};

char *KeyGet(link x) {
    return x->val.cod;
}

int KeyGreater(char *cod1, char *cod2) {
    if(strcmp(cod1, cod2) > 0)
        return 1;
    return 0;
}

int KeyEq(char *cod1, char *cod2) {
    if(strcmp(cod1, cod2) == 0)
        return 1;
    return 0;
}

int KeyGeq(char *cod1, char *cod2) {
    if(strcmp(cod1, cod2) >= 0)
        return 1;
    return 0;
}

void list_freeR(link h) {
    if(h == NULL)
        return;
    list_freeR(h->next);
    pg_clean(&(h->val));
    free(h);
}

pgList_t pgList_init() {
    pgList_t tabPg = malloc(sizeof(struct pgList_s));

    tabPg->headPg = NULL;
    tabPg->tailPg = NULL;

    tabPg->nPg = 0;

    return tabPg;
}

void pgList_free(pgList_t pgList) {
    list_freeR(pgList->headPg);
    free(pgList);
}

void pgList_read(FILE *fp, pgList_t pgList) {
    pg_t pg;
    while(pg_read(fp, &pg) != 0) {
        pgList_insert(pgList, pg);
        pgList->nPg++;
    }
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray) {
    for(link x = pgList->headPg; x != NULL; x = x->next) {
        pg_print(fp, &(x->val), invArray);
    }
}

void pgList_insert(pgList_t pgList, pg_t pg) {
    link x, p;
    if(pgList->headPg == NULL) {
        pgList->headPg = newNode(pgList->headPg, pg);
        pgList->tailPg = pgList->headPg;
        return;
    }
    else if(KeyGreater(KeyGet(pgList->headPg), pg.cod)) {
        pgList->headPg = newNode(pgList->headPg, pg);
        return;
    }
    for(x = pgList->headPg->next, p = pgList->headPg; x != NULL && KeyGreater(pg.cod, KeyGet(x)); p = x, x = x->next);
    p->next = newNode(x, pg);
    if(x == NULL)
        pgList->tailPg = p->next;
}

void pgList_remove(pgList_t pgList, char* cod) {
    link x, p;

    if(pgList->headPg == NULL || KeyGreater(KeyGet(pgList->headPg), cod) || KeyGreater(cod, KeyGet(pgList->tailPg)))
        return;
    for(x = pgList->headPg, p = NULL; x != NULL && KeyGeq(cod, KeyGet(x)); p = x, x = x->next) {
        if (KeyEq(KeyGet(x), cod)) {
            if (p == NULL)
                pgList->headPg = x->next;
            else {
                p->next = x->next;
                if(p->next == NULL)
                    pgList->tailPg = p;
            }
            pg_clean(&(x->val));
            free(x);
            break;
        }
    }
}

pg_t *pgList_searchByCode(pgList_t pgList, char* cod) {
    link x;

    if(pgList->headPg == NULL || KeyGreater(KeyGet(pgList->headPg), cod) || KeyGreater(cod, KeyGet(pgList->tailPg)))
        return NULL;
    for(x = pgList->headPg; x != NULL && KeyGreater(cod, KeyGet(x)); x = x->next);

    if (x != NULL && KeyEq(cod, KeyGet(x)))
        return &(x->val);

    return NULL;
}