#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int inizio,
    fine;
} att;

void riempi_struct(FILE *fin, int *N, att **v);
void attSel(int N, att *v);
void powerset(int N, att *v, int *sol, int pos, int *max, int dur, int *bsol);
int ok(att *v, int *sol, int N);
int durata(att *v, int *sol, int N);

int main() {
    FILE *fin = NULL;
    att *v;
    int N;

    riempi_struct(fin, &N, &v);
    attSel(N, v);

    free(v);

    return 0;
}

void riempi_struct(FILE *fin, int *N, att **v) {
    fin = fopen("att1.txt", "r");
    fscanf(fin, "%d\n", N);
    *v = (att *) malloc(*N * sizeof(att));

    for(int i = 0; i < *N; i++) {
        fscanf(fin, "%d %d\n", &((*v+i)->inizio), &((*v+i)->fine));
    }
    fclose(fin);
}
void attSel(int N, att *v) {
    int *sol, pos = 0, d_max = 0, dur = 0, *bsol;

    sol = (int *) malloc(N * sizeof(int));
    bsol = (int *) malloc(N * sizeof(int));
    powerset(N, v, sol, pos, &d_max, dur, bsol);

    printf("Sottoinsieme di attivita' compatibili che massimizza la durata:\n");
    for(int i = 0; i < N; i++) {
        if(bsol[i] == 1) {
            printf("(%d,%d) ", (v+i)->inizio, (v+i)->fine);
        }
    }
    printf("\n%d", d_max);


    free(sol);
    free(bsol);
}

void powerset(int N, att *v, int *sol, int pos, int *max, int dur, int *bsol) {

    if(pos >= N){
        dur = durata(v, sol, N);
        if(dur > *max) {
            *max = dur;
            for(int i = 0; i < N; i++) {
                if(sol[i] == 1)
                    bsol[i] = 1;
                else
                    bsol[i] = 0;
            }
        }
        return;
    }


    sol[pos] = 1;
    if(ok(v, sol, N))
        powerset(N, v, sol, pos + 1, max, dur, bsol);
    sol[pos] = 0;
    powerset(N, v, sol, pos + 1, max, dur, bsol);
}

int ok(att *v, int *sol, int N) {
    for(int i = 0; i < N - 1; i++) {
        if(sol[i] == 1) {
            for(int j = i + 1; j < N; j++) {
                if((v+i)->inizio < (v+j)->fine && (v+j)->inizio < (v+i)->fine && sol[j] == 1)
                    return 0;
            }
        }
    }
    return 1;
}

int durata(att *v, int *sol, int N) {
    int d = 0;
    for(int i = 0; i < N; i++) {
        if(sol[i] == 1) {
            d = d + (v+i)->fine - (v+i)->inizio;
        }
    }
    return d;
}