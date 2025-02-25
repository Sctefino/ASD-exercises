#include <stdio.h>
#include <ctype.h>
#include <string.h>


#include "inventario.h"
#include "personaggi.h"

typedef enum {
    agg_pers_el, agg_ogg_el, agg_pers, del_pers, mod_equip, stats, end, err
} comando;

comando menu();
void smista(comando x, tabInv_t **tabInv, tabPg_t **tabPg, int *flag_p, int *flag_i);
void low(char *stringa);

int main() {
    tabInv_t *tabInv = NULL;
    tabPg_t *tabPg = NULL;
    comando x;
    int flag_p = 1, flag_i = 1;
    do {
        x = menu();
        smista(x, &tabInv, &tabPg, &flag_p, &flag_i);
    } while(x != end);

    return 0;
}

comando menu() {
    char com[30]; //tutti i comandi sono più piccoli di questa dimensione
    printf("Per scegliere comando, digitare una delle seguenti scelte:\n"
           "Aggiungi personaggi elenco\n"
           "Aggiungi oggetti elenco\n"
           "Aggiungi personaggio\n"
           "Elimina personaggio\n"
           "Modifica equipaggiamento\n"
           "Calcolo statistiche\n"
           "Fine\n");
    gets(com);
    fflush(stdin);
    low(com);

    if(strcmp(com, "aggiungi personaggi elenco") == 0)
        return agg_pers_el;
    else if(strcmp(com, "aggiungi oggetti elenco") == 0)
        return agg_ogg_el;
    else if(strcmp(com, "aggiungi personaggio") == 0)
        return agg_pers;
    else if(strcmp(com, "elimina personaggio") == 0)
        return del_pers;
    else if(strcmp(com, "modifica equipaggiamento") == 0)
        return mod_equip;
    else if(strcmp(com, "calcolo statistiche") == 0)
        return stats;
    else if(strcmp(com, "fine") == 0)
        return end;
    else
        return err;

}

void smista(comando x, tabInv_t **tabInv, tabPg_t **tabPg, int *flag_p, int *flag_i) {
    FILE *fp_p;
    char code[7], name[51];
    int choice;
    link a;

    switch(x) {
        case agg_pers_el:
            fp_p = fopen("pg.txt", "r");
            while(!feof(fp_p)) {
                ins_pers(fp_p, tabPg, flag_p);
            }
            fclose(fp_p);
            break;
        case agg_ogg_el:
            if(!flag_i)
                printf("Oggetti già inseriti in inventario!");
            else
                ins_ogg(tabInv, flag_i);
            break;
        case agg_pers:
            printf("Inserire dettagli personaggio da inserire:\n");
            ins_pers(stdin, tabPg, flag_p);
            break;
        case del_pers:
            printf("Inserire codice personaggio da eliminare:\n");
            scanf("%s", code);
            fflush(stdin);
            del_pg(*tabPg, code);
            break;
        case mod_equip:
            printf("Inserire codice personaggio:\n");
            scanf("%s", code);
            fflush(stdin);
            printf("Inserire nome oggetto:\n");
            scanf("%s", name);
            fflush(stdin);
            printf("Aggiungere (1) o rimuovere(0)?\n");
            scanf("%d", &choice);
            fflush(stdin);
            mod_equipa(*tabPg, *tabInv, name, code, choice);
            break;
        case stats:
            printf("Inserire codice personaggio:\n");
            scanf("%s", code);
            fflush(stdin);
            a = search_pg(*tabPg, code);
            if(a != NULL) {
                for (int i = 0; i < 8; i++) {
                    if (a->val.equip->vettEq[i] != NULL) {
                        for (int j = 0; j < 6; j++) {
                            a->val.stat[j] += a->val.equip->vettEq[i]->stat[j];
                        }
                    }
                }
                stampa_pg(*tabPg, *tabInv, code);
                //Per non falsare i successivi conti, e aggiungere di nuovo un oggetto già considerato,
                //e vista la bassa complessità computazionale che questa operazione raffigura in questa
                //situazione (O(48)), scelgo di togliere gli effetti di tutti gli oggetti dopo la stampa
                for (int i = 0; i < 8; i++) {
                    if (a->val.equip->vettEq[i] != NULL) {
                        for (int j = 0; j < 6; j++) {
                            a->val.stat[j] -= a->val.equip->vettEq[i]->stat[j];
                        }
                    }
                }
            }
            else
                printf("Personaggio con quel codice non trovato!\n");
            break;
        case end:
            deall_pg(*tabPg, flag_p);
            deall_inv(*tabInv, flag_i);
            break;
        case err:
            printf("Comando non inserito correttamente!\n");
            break;
        default:
            printf("Errore!\n");
            break;
    }
}

void low(char *stringa) {
    for(int i = 0; stringa[i] != '\0'; i++) {
        if(stringa[i] >= 'A' && stringa[i] <= 'Z') {
            stringa[i] = tolower(stringa[i]);
        }
    }
}