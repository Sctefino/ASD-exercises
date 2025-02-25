#include <stdio.h>
#include "Item.h"

int KEYcmp(float A, float B) {
    if(A<B)
        return -1;
    if(A>B)
        return 1;
    return 0;
}

float KEYGet(Item A) {
    return (float)A.val/A.diff;
}

void ITEMStore(Item A) {
    printf("%s %d %d %d %d %d %f %d\n", A.name, A.tipo, A.ingr, A.usc, A.prec, A.fin, A.val, A.diff);
}

int item_start(Item A) {
    return (A.ingr == 1 && A.prec == 0);
}

int diff_item(Item A) {
    return A.diff;
}

int check_val(Item A, Item B, int sumDD, int sumDP, int DD, int DP, int start) { //se è l'elemento iniziale, devo solo controllare somma diff
    return start == 0? (B.diff + sumDD < DD && B.diff + sumDP < DP) : (A.usc == B.ingr && B.diff + sumDD < DD && B.diff + sumDP < DP);
}

int item_acr_ind(Item A) {
    return(A.tipo == 1);
}

int item_acr_front(Item A) {
    return(A.tipo == 2);
}

int item_usc(Item A) {
    return A.usc;
}