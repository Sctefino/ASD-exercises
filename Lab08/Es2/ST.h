#ifndef ES2_ST_H
#define ES2_ST_H

#define MAXS 30

typedef struct symboltable *ST;

ST STinit();
void STfree(ST st);
void STinsert(ST st, char name[MAXS]);
int STsize(ST st);
void STstore(ST st);
int STsearch(ST st, char name[MAXS]);
char *STsearch_by_index(ST st, int index);
#endif //ES2_ST_H
