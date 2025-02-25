#ifndef ES3_TITOLO_H
#define ES3_TITOLO_H
#include <stdio.h>


#define MAXC 20

typedef struct title *titolo;

titolo titinit(int n);
void freetitle(titolo A);
titolo read_title(FILE *fin);
void titstore(titolo A);
char *getname(titolo A);
int titcmp(titolo A, titolo B);
void merge_title(titolo A, titolo B);
void find_quot(titolo t, int a, int m, int g);
void find_quot_date(titolo t, int a, int m, int g, int a1, int m1, int g1);
void balance(titolo t, int S);

#endif //ES3_TITOLO_H
