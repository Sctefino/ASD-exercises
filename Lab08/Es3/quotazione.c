#include "quotazione.h"

quotazione quotvoid() {
    quotazione q;
    q.date = datavoid();
    q.quot = 0;
    q.N = 0;

    return q;
}

data quotdate(quotazione q) {
    return q.date;
}

quotazione getquot(quotazione q, data date, int n, int v) {

    q.date = date;
    q.quot = (q.quot * (float)q.N + (float)(n * v))/(float)(q.N + n);
    q.N = q.N + n;

    return q;
}

float quotval(quotazione q) {
    return q.quot;
}