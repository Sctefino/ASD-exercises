#include <stdio.h>
#include <stdlib.h>
#include "PQ.h"
#include "diag.h"

PQ riempi_coda(FILE *fp, PQ pq, int *n);
void calcola_diagonali(PQ pq, DIAG *diagonali, int DD, int DP);

int main(int argc, char **argv) {
    int DD, DP, n;
    PQ pq = NULL;
    FILE *fp = NULL;
    DIAG diagonali;


    pq = riempi_coda(fp, pq, &n);

    for(int i = 1; i < argc; i+=2) {
        diagonali = diag_dim_zero(diagonali);
        DD = atoi(argv[i]);
        DP = atoi(argv[i+1]);
        calcola_diagonali(pq, &diagonali, DD, DP);
        diag_print(diagonali);
    }


    PQfree(pq);
    return 0;
}

PQ riempi_coda(FILE *fp, PQ pq, int *n) {
    Item A;

    fp = fopen("elementi.txt", "r");

    fscanf(fp, "%d\n", n);
    pq = PQinit(*n);

    for(int i = 0; i < *n; i++) {
        fscanf(fp, "%s %d %d %d %d %d %f %d\n", A.name, &(A.tipo), &(A.ingr), &(A.usc), &(A.prec),
               &(A.fin), &(A.val), &(A.diff));
        PQinsert(pq, A);
    }

    return pq;
}

void calcola_diagonali(PQ pq, DIAG *diagonali, int DD, int DP) {

    int sumDD = 0, sumDP = 0, index = 0, flag_front = 0;
    //terza diag: ragiono prendendo sempre il miglior esercizio, tra quelli compatibili con le regole, (miglior esercizio = rapporto valore/diff più alto)
    //ottenendo così la diagonale a punteggio più alto delle 3 (in linea teorica). Mi segno se l'elemento acrobatico obbligatorio
    //di questo esercizio era avanti o dietro, così alla prossima diagonale metto uno del verso opposto
    //seconda diag: faccio una diag con due esercizi acrobatici in fila, sempre prendendo i valori migliori
    //prima diag: tutte le regole particolari sono state completate, quindi ne creo una che segua le regole generali (DD, DP, acrobatico, inizio...)

    for(int n = NUMDIAG-1; n >= 0; n--) {
        //elemento iniziale
        for (int i = 0; i < PQsize(pq); i++) {
            if (item_start(el_index(pq, i)) &&
                check_val(el_index(pq, i), el_index(pq, i), sumDD, sumDP, DD, DP, index)) {
                if(n != NUMDIAG - 2 || flag_front != item_usc(el_index(pq,i))) {
                    el_insert(diagonali, el_index(pq, i), n);
                    index++;
                    sumDD += diff_item(el_index(pq, i));
                    sumDP += diff_item(el_index(pq, i));
                    break;
                }
            }
        }
        //forzo la presa di un elemento elemento acrobatico
        //se calcolo seconda diagonale ne prendo due di fila, uno avanti che ha uscita dietro e uno dietro o viceversa
        for (int i = 0; i < PQsize(pq); i++) {
            if ((item_acr_ind(el_index(pq,i)) || item_acr_front(el_index(pq,i))) && check_val(diagonali->diag[n][index - 1], el_index(pq, i), sumDD, sumDP, DD, DP, index)) {
                flag_front = item_acr_front(el_index(pq,i));
                el_insert(diagonali, el_index(pq, i), n);
                index++;
                sumDD += diff_item(el_index(pq, i));
                sumDP += diff_item(el_index(pq, i));
                break;
            }
        }
        //prendo il secondo elemento acrobatico in fila solo sulla seconda diagonale, se prima ho trovato front, prendo ind, e viceversa
        for (int i = 0; i < PQsize(pq) && n == NUMDIAG - 2; i++) {
            if ((item_acr_ind(el_index(pq,i)) || item_acr_front(el_index(pq,i))) && check_val(diagonali->diag[n][index - 1], el_index(pq, i), sumDD, sumDP, DD, DP, index)) {
                el_insert(diagonali, el_index(pq, i), n);
                index++;
                sumDD += diff_item(el_index(pq, i));
                sumDP += diff_item(el_index(pq, i));
                break;
            }
        }
        //una volta rispettate le regole, riempio la diagonale finché possibile
        for (int i = 0; i < PQsize(pq); i++) {
            if (check_val(diagonali->diag[n][index - 1], el_index(pq, i), sumDD, sumDP, DD, DP, index)) {
                el_insert(diagonali, el_index(pq, i), n);
                index++;
                sumDD += diff_item(el_index(pq, i));
                sumDP += diff_item(el_index(pq, i));
            }
        }
        sumDD = 0;
        index = 0;
    }
}