#ifndef ES3_ORA_H
#define ES3_ORA_H

typedef struct {
    int ore;
    int minuti;
} ora;

ora getora(int o, int min);
void orastore(ora hour);
#endif //ES3_ORA_H
