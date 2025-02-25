#ifndef ES1_DIAG_H
#define ES1_DIAG_H
#include "Item.h"
#define DMAX 5
#define NUMDIAG 3

typedef struct diags {
    Item diag[NUMDIAG][DMAX];
    int dim[NUMDIAG]; //per indicare la dimensione effettiva di ogni diagonale
} DIAG;

void el_insert(DIAG *diag, Item item, int num_diag);
DIAG diag_dim_zero(DIAG diag);
void diag_print(DIAG diag);
#endif //ES1_DIAG_H
