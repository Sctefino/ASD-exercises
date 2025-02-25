#include "diag.h"
#include "Item.h"
#include <stdio.h>

DIAG diag_dim_zero(DIAG diag) {
    for(int i = 0; i < DMAX; i++) {
        diag.dim[i] = 0;
    }
    return diag;
}

void el_insert(DIAG *diag, Item item, int num_diag) {
    diag->diag[num_diag][diag->dim[num_diag]] = item;
    diag->dim[num_diag]++;
}

void diag_print(DIAG diag) {
    for(int i = 0; i < NUMDIAG; i++) {
        printf("DIAG %d:\n", i+1);
        for(int j = 0; j < diag.dim[i]; j++) {
            ITEMStore(diag.diag[i][j]);
        }
        printf("\n");
    }
}