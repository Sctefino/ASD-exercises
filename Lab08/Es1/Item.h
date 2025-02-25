#ifndef ES1_ITEM_H
#define ES1_ITEM_H

#define LMAX 100

typedef struct {
    char name[LMAX];
    int tipo,
    ingr,
    usc,
    prec,
    fin;
    float val;
    int diff;
} Item;

int KEYcmp(float A, float B);
float KEYGet(Item A);
void ITEMStore(Item A);
int item_start(Item A);
int item_acr_front(Item A);
int item_acr_ind(Item A);
int diff_item(Item A);
int item_usc(Item A);
int check_val(Item A, Item B, int sumDD, int sumDP, int DD, int DP, int start);
#endif //ES1_ITEM_H