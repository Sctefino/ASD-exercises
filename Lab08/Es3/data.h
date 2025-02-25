#ifndef ES3_DATA_H
#define ES3_DATA_H

typedef struct {
    int anno;
    int mese;
    int giorno;
} data;

int datacmp(data A, data B);
data getdata(int a, int mes, int g);
data datavoid();
void datastore(data date);
#endif //ES3_DATA_H