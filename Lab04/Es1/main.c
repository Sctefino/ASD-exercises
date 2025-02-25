#include <stdio.h>
#include <stdlib.h>

void malloc2dP(int ***mp, int nr);
void free2dP(int **mp, int nr);
void powerset(int pos, int *val, int *sol, int n, int start, int cnt, int **archi, int E);
int check(int **archi, int *sol, int n, int E);

int main() {
    FILE *fin;
    int N, E, *vert, **archi, *sol;

    fin = fopen("grafo.txt", "r");
    fscanf(fin, "%d %d\n", &N, &E);

    vert = (int *) malloc(N*sizeof(int));
    malloc2dP(&archi, E);
    sol = (int *) malloc(N*sizeof(int));

    for(int i = 0; i < N; i++) {
        vert[i] = i;
    }
    for(int i = 0; i < E; i++) {
        fscanf(fin, "%d %d\n", &archi[i][0], &archi[i][1]);
    }
    powerset(0, vert, sol, N, 0, 0, archi, E);

    free(vert);
    free2dP(archi, E);

    free(sol);
    fclose(fin);
    return 0;
}

void powerset(int pos, int *vert, int *sol, int n, int start, int cnt, int **archi, int E) {
    int i;
    if (start >= n) {
        if(check(archi, sol, pos, E)) {
            for (i = 0; i < pos; i++)
                printf("%d ", sol[i]);
            printf("\n");
        }
        return;
    }
    for (i = start; i < n; i++) {
        sol[pos] = vert[i];
        powerset(pos+1, vert, sol, n, i+1, cnt, archi, E);
    }
    powerset(pos, vert, sol, n, n, cnt, archi, E);
    return;
}

void malloc2dP(int ***mp, int nr) {
    *mp = (int **) malloc(nr*sizeof(int *));
    for(int i = 0; i < nr; i++) {
        (*mp)[i] = (int *) malloc(2 * sizeof(int));
    }
}

void free2dP(int **mp, int nr) {
    for(int i = 0; i < nr; i++) {
        free(mp[i]);
    }
    free(mp);
}

int check(int **archi, int *sol, int n, int E) {
    int *mark;
    mark = calloc(E, sizeof(int));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < E; j++) {
            if(sol[i] == archi[j][0] || sol[i] == archi[j][1])
                mark[j] = 1;
        }
    }
    for(int i = 0; i < E; i++) {
        if(mark[i] == 0) {
            free(mark);
            return 0;
        }
    }
    free(mark);
    return 1;
}