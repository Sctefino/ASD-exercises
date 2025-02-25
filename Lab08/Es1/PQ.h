#ifndef ES1_PQ_H
#define ES1_PQ_H

#include "Item.h"

typedef  struct pqueue* PQ;

PQ PQinit(int maxN);
void PQfree(PQ pq);
int PQempty(PQ pq);
void PQinsert(PQ pq, Item val);
//Item PQextractMax(PQ pq);
Item PQshowMax(PQ pq);
void PQdisplay(PQ pq);
int PQsize(PQ pq);
Item el_index(PQ pq, int index);
//void PQchange(PQ pq, Item val);

#endif //ES1_PQ_H
