#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **scelte;
    int num_scelte;
} Livello;

void alloc(Livello **val, FILE *fin, int *n_rag, char ***sol);
int princ_molt(int pos, Livello *val, char **sol, int n, int cnt);
void malloc2dp(char ***mp, int nr);
void free2d(char **mp, int nr);

int main() {
    FILE *fpin;
    Livello *val;
    char **sol;
    int n_rag, cnt = 0, pos = 0;

    fpin = fopen("brani.txt", "r");

    if(fpin == NULL) {
        printf("Errore apertura file di input");
        return 1;
    }
    alloc(&val, fpin, &n_rag, &sol);

    cnt = princ_molt(pos, val, sol, n_rag, cnt);
    printf("%d", cnt);

    fclose(fpin);
    free2d(sol, n_rag);
    free(val);

    return 0;
}

void alloc(Livello **val, FILE *fin, int *n_rag, char ***sol) {

    //Leggo il numero di livelli, e alloco il vettore di struct livello
    fscanf(fin, " %d ", n_rag);

    malloc2dp(sol, *n_rag);
    *val = (Livello *) malloc(*n_rag*sizeof(Livello));
    if(*val == NULL)
        exit(2);

    for(int i = 0; i < *n_rag; i++) {
        //Prendo il numero di canzoni per ragazzo, lo inserisco nel vettore e nel campo, alloco il vettore scelte e le stringhe
        //per memorizzare i brani
        fscanf(fin, " %d ", &((*val)[i].num_scelte));
        (*val)[i].scelte = malloc(((*val)[i].num_scelte)*sizeof(char *));
        if((*val)[i].scelte == NULL)
            exit(2);
        //Inserisco le canzoni nelle stringhe apposite
        for(int j = 0; j < (*val)[i].num_scelte; j++) {
            ((*val)[i].scelte)[j] = malloc(256*sizeof(char));
            if(((*val)[i].scelte)[j] == NULL)
                exit(2);
            fscanf(fin, " %s ", ((*val)[i].scelte)[j]);
        }
    }
}

int princ_molt(int pos, Livello *val, char **sol, int n, int cnt) {
    int i;
    if (pos >= n) {
        for (i = 0; i < n; i++)
            printf("%s ", sol[i]);
        printf("\n");
        return cnt+1;
    }
    for (i = 0; i < val[pos].num_scelte; i++) {
        strcpy(sol[pos], val[pos].scelte[i]);
        cnt = princ_molt(pos+1, val, sol, n, cnt);
    }
    return cnt;
}

void malloc2dp(char ***mp, int nr) {
    *mp = malloc(nr*sizeof(char *));
    if((*mp) == NULL)
        exit(2);
    for(int i = 0; i < nr; i++) {
        (*mp)[i] = malloc(256*sizeof(char));
        if((*mp)[i] == NULL)
            exit(2);
    }
}

void free2d(char **mp, int nr) {
    for(int i = 0; i < nr; i++){
        free(mp[i]);
    }
    free(mp);
}