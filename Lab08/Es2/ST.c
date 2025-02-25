#include "ST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct symboltable {
    char (*name)[MAXS];
    int n;
    int max;
};

ST STinit() {
    ST st;

    st = malloc(sizeof(*st));
    st->name = malloc(sizeof(*(st->name)));
    st->n = 0;
    st->max = 1;

    return st;
}

void STfree(ST st) {
    free(st->name);
    free(st);
}

int STsize(ST st) {
    return st->n;
}

void STstore(ST st) {
    for(int i = 0; i < st->n; i++) {
        printf("%s ", st->name[i]);
    }
}

static int check_double(ST st, char name[MAXS]) {
    for(int i = 0; i < st->n; i++) {
        if(strcmp(name, st->name[i]) == 0)
            return 1;
    }
    return 0;
}

void STinsert(ST st, char name[MAXS]) {

    if(check_double(st, name))
        return;

    int i = st->n++;

    if(st->n > st->max) {
        st->max = 2*st->max;
        st->name = realloc(st->name, st->max*sizeof(*(st->name)));
        if(st->name ==NULL)
            return;
    }
    while(i > 0 && strcmp(name, st->name[i-1]) < 0) {
        strcpy(st->name[i], st->name[i-1]);
        i--;
    }

    strcpy(st->name[i], name);
}

int STsearch(ST st, char name[MAXS]) {
    int i;

    for(i = 0; i < st->n; i++)
        if(strcmp(st->name[i], name) == 0)
            return i;

    return -1;
}

char *STsearch_by_index(ST st, int index) {
    return st->name[index];
}