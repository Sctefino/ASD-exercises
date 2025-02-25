#include "data.h"
#include <stdio.h>

int datacmp(data A, data B) {
    int x, y;
    x = A.anno * 10000 + A.mese * 100 + A.giorno;
    y = B.anno * 10000 + B.mese * 100 + B.giorno;

    if(x < y)
        return -1;
    else if(x > y)
        return 1;
    return 0;
}

data getdata(int a, int mes, int g) {
    data d;

    d.anno = a;
    d.mese = mes;
    d.giorno = g;

    return d;
}

data datavoid() {
    data d;

    d.anno = 0;
    d.mese = 0;
    d.giorno = 0;

    return d;
}

void datastore(data date) {
    printf("%d/%d/%d ", date.anno, date.mese, date.giorno);
}