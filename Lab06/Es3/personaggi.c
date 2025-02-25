#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaggi.h"

void free_equip(link x);

link newNode(pg_t value, link next) {
    link x = malloc(sizeof(nodoPg_t));

    if (x==NULL)
        return NULL;
    else {
        x->val = value;
        x->val.equip = malloc(sizeof(tabEquip_t));
        x->val.equip->inUso = 0;
        x->val.equip->vettEq = calloc(8, sizeof(inv_t *));
        x->next = next;
    }
    return x;
}

link SortListIns(link h, link *t, pg_t value, int *flag_sum) {
    link x, p;

    if (h == NULL || strcmp(h->val.codice, value.codice) > 0) {
        if (*t == NULL) {
            h = *t = newNode(value, h);
            return h;
        }
        return newNode(value, h);
    }

    for (x = h->next, p = h;
         x != NULL && strcmp(value.codice, x->val.codice) >= 0;
         p = x, x = x->next);

    if (strcmp(p->val.codice, value.codice) == 0) {
        //considerando il codice come univoco, evito di inserire duplicati
        *flag_sum = 0;
        return h;
    }
    p->next = newNode(value, x);
    if (x == NULL)
        *t = p->next;
    return h;
}

void ins_pers(FILE *fp, tabPg_t **tabPg, int *flag) {
    pg_t temp;
    int flag_sum = 1;

    if(*flag) {
        *tabPg = (tabPg_t *) malloc(sizeof(tabPg_t));
        (*tabPg)->headPg = (link) malloc(sizeof(nodoPg_t));
        (*tabPg)->tailPg = (link) malloc(sizeof(nodoPg_t));
        (*tabPg)->headPg = (*tabPg)->tailPg = NULL;
        (*tabPg)->nPg = 0;
        *flag = 0; //per segnare la prima volta che inserisco dati nella lista
    }

    fscanf(fp, "%s %s %s %d %d %d %d %d %d ", temp.codice, temp.nome, temp.classe, temp.stat, temp.stat + 1,
           temp.stat + 2, temp.stat + 3, temp.stat + 4, temp.stat +5);
    if(fp != stdin)
        fscanf(fp, "\n");
    else
        fflush(stdin);
    (*tabPg)->headPg = SortListIns((*tabPg)->headPg, &((*tabPg)->tailPg), temp, &flag_sum);
    if(flag_sum)
        (*tabPg)->nPg ++;

}

link SortListDel(link h, char *stringa, int *flag_del) {
    link x, p;
    if (h == NULL)
        return NULL;
    for (x=h, p=NULL; x!=NULL && strcmp(stringa, x->val.codice) >= 0;
         p=x, x=x->next) {
        if (strcmp(x->val.codice ,stringa) == 0){
            if (x==h)
                h = x->next;
            else
                p->next = x->next;
            *flag_del = 1;
            free_equip(x);
            free(x); //da vedere perché errore in debug
            break;
        }
    }
    return h;
}

void del_pg(tabPg_t *tabPg, char *code) {
    int flag_del = 0;
    tabPg->headPg = SortListDel(tabPg->headPg, code, &flag_del);
    if(flag_del) {
        tabPg->nPg--;
    }
}

link search_pg(tabPg_t *tabPg, char *code) { //restituisce puntatore al nodo se trova, o NULL se non trova
    link x;
    for (x=tabPg->headPg;
         x!=NULL && strcmp(code, x->val.codice) >= 0;
         x=x->next)
        if (strcmp(x->val.codice, code) == 0)
            return x;
    return NULL;
}

void stampa_equip(tabInv_t *tabInv, inv_t **vetteq) {
    printf("Equipaggiamento:\n");
    for(int i = 0; i < 8; i++) {
        if(vetteq[i] != NULL)
            stampa_ogg(tabInv, (vetteq[i])->nome, 10);
    }
}

void stampa_pg(tabPg_t *tabPg, tabInv_t *tabInv, char *code) {
    link x;

    x = search_pg(tabPg, code);
    if(x == NULL)
        printf("Nessun personaggio con quel codice presente!\n");
    else {
        printf("Codice: %s\n"
               "Nome: %s\n"
               "Classe: %s\n",
               x->val.codice, x->val.nome, x->val.classe, x->val);
        if(x->val.equip->inUso == 0) {
            printf("Equipaggiamento: NaN\n");
        }
        else
            stampa_equip(tabInv, x->val.equip->vettEq);
        printf("Stats: %d %d %d %d %d %d\n", x->val.stat[0] < 0? 0:x->val.stat[0],
               x->val.stat[1] < 0? 0:x->val.stat[1],
               x->val.stat[2] < 0? 0:x->val.stat[2],
               x->val.stat[3] < 0? 0:x->val.stat[3],
               x->val.stat[4] < 0? 0:x->val.stat[4],
               x->val.stat[5] < 0? 0:x->val.stat[5]);
    }
}

void mod_equipa(tabPg_t *tabPg, tabInv_t *tabInv, char *name, char *code, int choice) {
    link x;
    int index, y;
    x = search_pg(tabPg, code);
    if(x == NULL) {
        printf("Personaggio non trovato!\n");
        return;
    }

    if(choice) {
        if(x->val.equip->inUso == 8) {
            printf("Equipaggiamento pieno!\n");
            return;
        }
        index = search_ogg(tabInv, name);
        if(index == -1) {
            printf("Oggetto non trovato!\n");
            return;
        }
        for(y = 0; y < x->val.equip->inUso && x->val.equip->vettEq[y] != NULL; y++); //appena trovo un buco con NULL, o dopo
        //tutti quelli in uso, mi fermo
        x->val.equip->vettEq[y] = tabInv->vettInv + index;
        x->val.equip->inUso ++;
    }
    else {
        if(x->val.equip->inUso == 0) {
            printf("Equipaggiamento vuoto!\n");
            return;
        }
        for(int i = 0; i < 8; i++) { //devo vederli tutti e 8 perché potrebbero esserci buchi
            if(x->val.equip->vettEq[i] != NULL && strcmp(x->val.equip->vettEq[i]->nome, name) == 0) {
                x->val.equip->vettEq[i] = NULL;
                x->val.equip->inUso --;
            }
        }
    }
}
void free_equip(link x) {

    free(x->val.equip->vettEq);
    free(x->val.equip);
}
void free_nodes(link x) {
    if(x == NULL)
        return;
    free_nodes(x->next);
    free_equip(x);
    free(x);
}
void deall_pg(tabPg_t *tabPg, int *flag) {
    if(!*flag) {
        free_nodes(tabPg->headPg); //così libero anche head e tail
        free(tabPg);
    }
}