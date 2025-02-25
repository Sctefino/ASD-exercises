#include "coll_titoli.h"
#include <string.h>
#include <stdlib.h>

typedef struct node *link;

struct node {
    titolo title;
    link next;
};

struct collezione {
    link head;
};

static link NEWnode(titolo x, link next) {
    link h;

    h = malloc(sizeof(*h));
    h->title = x;
    h->next = next;

    return h;
}

coll_titoli collinit() {
    coll_titoli x;

    x = malloc(sizeof(*x));
    x->head = NULL;

    return x;
}

void file_read(coll_titoli lista, FILE *fin) {
    int n;
    link h, p;
    titolo x;

    fscanf(fin, "%d\n", &n);

    for(int i = 0; i < n; i++) {
        x = read_title(fin);
        for(h = lista->head, p = NULL; h != NULL && titcmp(x, h->title) >= 0; p = h, h = h->next);
        if(p == NULL)
            lista->head = NEWnode(x, h);
        else if(titcmp(p->title, x) == 0)
            merge_title(p->title, x);
        else
            p->next = NEWnode(x, h);

    }
}

titolo searchTitle(coll_titoli lista, char name[MAXC]) {
    if(lista->head == NULL || strcmp(getname(lista->head->title), name) > 0)
        return NULL;
    for(link h = lista->head; h != NULL && strcmp(getname(lista->head->title), name) <= 0; h = h->next) {
        if(strcmp(getname(h->title), name) == 0)
            return h->title;
    }
    return NULL;
}
void freenode(link h) {
    if(h == NULL)
        return;

    freenode(h->next);
    freetitle(h->title);
    free(h);
}

void freecoll(coll_titoli lista) {
    freenode(lista->head);
    free(lista);
}