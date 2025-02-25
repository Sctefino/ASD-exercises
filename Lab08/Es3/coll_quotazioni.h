#ifndef ES3_COLL_QUOTAZIONI_H
#define ES3_COLL_QUOTAZIONI_H
#include "data.h"

typedef struct binarysearchtreeq *BSTq;

BSTq bstinit();
void BSTfree(BSTq bst);
void BSTinsert(BSTq bst, data date, int n, int v);
void BSTsearch(BSTq bst, data date);
void BSTsearch_int(BSTq bst, data date1, data date2);
void count_path(BSTq bst, int *max, int *min);
void BSTbalance(BSTq bst);

#endif //ES3_COLL_QUOTAZIONI_H
