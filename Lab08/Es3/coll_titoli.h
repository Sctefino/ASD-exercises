#ifndef ES3_COLL_TITOLI_H
#define ES3_COLL_TITOLI_H
#include "titolo.h"

typedef struct collezione *coll_titoli;

coll_titoli collinit();
void file_read(coll_titoli lista, FILE *fin);
titolo searchTitle(coll_titoli, char name[MAXC]);
void freecoll(coll_titoli lista);

#endif //ES3_COLL_TITOLI_H
