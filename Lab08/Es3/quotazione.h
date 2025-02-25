#ifndef ES3_QUOTAZIONE_H
#define ES3_QUOTAZIONE_H

#include "data.h"

typedef struct {
    data date;
    float quot;
    int N;
} quotazione;

quotazione quotvoid();
quotazione getquot(quotazione q, data date, int n, int v);
data quotdate(quotazione q);
float quotval(quotazione q);
#endif //ES3_QUOTAZIONE_H
