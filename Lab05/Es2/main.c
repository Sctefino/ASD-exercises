#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char C[3];
    int P[2];
} tess;

int riempi_struct(tess **inp, FILE *fin, int *N);
int riempi_matr(int ***tab, FILE *fin, int *nr, int *nc);
int malloc2dP(int ***mat, int nr, int nc);
void game(tess *inp, int **table, int N, int nr, int nc);
void set_mark(int *mark_tab, int *mark_tess, int **table, int n);
void game_rec(tess *inp, int **table, int *mark_tab, int *mark_tess, int N, int nr, int nc, int pos, int *max, int **b_tab, int punti);
void ins_val(int *mark, int i, int val);
int points(tess *inp, int **table, int nr, int nc);
int sum(tess *inp, int **table, int num_r, int nr, int nc, char flag);
int check(tess *inp, int **table, int nr, int nc, int riga, char flag);
int get_point(tess *inp, int val, int rot, char flag);
void copy_tab(int **table, int **b_tab, int nr, int nc);
void print_tab(int **table, int nr, int nc);
void dealloca(tess *inp, int **table, int nr);

int main() {
    FILE *fin1 = NULL, *fin2 = NULL;
    tess *inp;
    int N, **table, nr, nc;

    if(riempi_struct(&inp, fin1, &N))
        return 1;

    if(riempi_matr(&table, fin2, &nr, &nc))
        return 1;

    game(inp, table, N, nr, nc);

    dealloca(inp, table, nr);

    return 0;
}

int riempi_struct(tess **inp, FILE *fin, int *N) {

    fin = fopen("tiles.txt", "r");
    if(fin == NULL)
        return 1;

    fscanf(fin, "%d\n", N);

    *inp = (tess *) malloc(*N * sizeof(tess));
    if(*inp == NULL)
        return 1;

    for(int i = 0; i < *N; i++) {
        fscanf(fin, "%c %d %c %d\n", &((*inp+i)->C[0]), &((*inp+i)->P[0]), &((*inp+i)->C[1]), &((*inp+i)->P[1]));
    }

    fclose(fin);

    return 0;
}

int riempi_matr(int ***tab, FILE *fin, int *nr, int *nc) {

    fin = fopen("board.txt", "r");
    if(fin == NULL)
        return 1;
    fscanf(fin, "%d %d\n", nr, nc);

    if(malloc2dP(tab, *nr, *nc))
        return 1;

    for(int i = 0; i < (*nr)*(*nc); i++) {
        fscanf(fin, "%d/%d ", &((*tab)[i][0]), &((*tab)[i][1]));
    }

    fclose(fin);

    return 0;
}

void game(tess *inp, int **table, int N, int nr, int nc) {
    int *mark_tab, *mark_tess, **b_tab, max = 0, punti = 0;

    mark_tab = (int *) calloc(nr*nc, sizeof(int));
    if(mark_tab == NULL)
        return;
    //Il vettore mark tess avrà il doppio del numero delle tessere, nelle caselle di indice pari
    //ci saranno quelle con rotazione 0, in quelle dispari rotazione 1.
    mark_tess = (int *) calloc(2*N, sizeof(int));
    if(mark_tess == NULL)
        return;
    if(malloc2dP(&b_tab, nr, nc))
        return;

    set_mark(mark_tab, mark_tess, table, nr*nc);
    game_rec(inp, table, mark_tab, mark_tess, N, nr, nc, 0, &max, b_tab, punti);

    print_tab(b_tab, nr, nc);
    printf("\n%d", max);

    free(mark_tab);
    free(mark_tess);

}

void set_mark(int *mark_tab, int *mark_tess, int **table, int n) {
    //Imposto le caselle della scacchiere già piene con valore 2, così da non toccarle nei controlli successivi
    for(int i = 0; i < n; i++) {
        if(table[i][0] != -1){
            mark_tab[i] = 2;
            //La stessa tessera con le due rotazioni occupa due caselle adiacenti nel vettore mark_tess
            ins_val(mark_tess, 2*(table[i][0]), 2);
        }

    }
}

void game_rec(tess *inp, int **table, int *mark_tab, int *mark_tess, int N, int nr, int nc, int pos, int *max, int **b_tab, int punti) {
    if(pos >= nr*nc) {

        punti = points(inp, table, nr, nc);

        if(punti > *max) {
            *max = punti;
            copy_tab(table, b_tab, nr, nc);
        }
        return;
    }

    if(mark_tab[pos] == 2)
        game_rec(inp, table, mark_tab, mark_tess, N, nr, nc, pos+1, max, b_tab, punti);
    else {
        for(int i = 0; i < 2*N; i++) {
            if(mark_tess[i] == 0) {
                ins_val(mark_tess, i, 1);
                table[pos][0] = i/2;
                table[pos][1] = i%2;
                game_rec(inp, table, mark_tab, mark_tess, N, nr, nc, pos+1, max, b_tab, punti);
                ins_val(mark_tess, i, 0);
            }
        }
    }
}

void ins_val(int *mark_tess, int i, int val) {
    mark_tess[i] = val;
    if(i % 2 == 0)
        mark_tess[i+1] = val;
    else
        mark_tess[i-1] = val;
}

int malloc2dP(int ***mat, int nr, int nc) {
    *mat = (int **) malloc(nr*nc*sizeof(int *));
    if(*mat == NULL)
        return 1;
    for(int i = 0; i < nr*nc; i++) {
        (*mat)[i] = (int *) malloc(2 * sizeof(int));
        if ((*mat)[i] == NULL)
            return 1;
    }
    return 0;
}

int points(tess *inp, int **table, int nr, int nc) {
    int punti = 0;

    for(int i = 0; i < nr; i++) {
        if(check(inp, table, nr, nc, i, 'r'))
            punti += sum(inp, table, i, nr, nc, 'r');
    }
    for(int j = 0; j < nc; j++) {
        if(check(inp, table, nr, nc, j, 'c'))
            punti += sum(inp, table, j, nr, nc, 'c');
    }

    return punti;
}

int sum(tess *inp, int **table, int num_r, int nr, int nc, char flag) {
    int somma = 0;

    for(int i = 0; i < nc && flag == 'r'; i++) {
        somma += get_point(inp, table[num_r * nr + i][0], table[num_r * nr + i][1], flag);
    }
    for(int i = 0; i < nr && flag == 'c'; i++) {
        somma += get_point(inp, table[num_r + nc * i][0], table[num_r + nc*i][1], flag);
    }
    return somma;
}

int check(tess *inp, int **table, int nr, int nc, int riga, char flag) {
    int rot1, rot2, val1, val2;

    for (int j = 0; j < nc - 1; j++) {
        if(flag == 'r') {
            rot1 = table[nr * riga + j][1];
            rot2 = table[nr * riga + j + 1][1];
            val1 = table[nr * riga + j][0];
            val2 = table[nr * riga + j + 1][0];
            if (inp[val1].C[rot1] != inp[val2].C[rot2])
                return 0;
        }
        else if(flag == 'c') {
            rot1 = table[riga + j * nc][1];
            rot2 = table[riga + (j+1) * nc][1];
            val1 = table[riga + j * nc][0];
            val2 = table[riga + (j+1) * nc][0];
            if (inp[val1].C[1 - rot1] != inp[val2].C[1 - rot2])
                return 0;
        }

    }

    return 1;
}

int get_point(tess *inp, int val, int rot, char flag) {

    return flag == 'r'? inp[val].P[rot]: inp[val].P[1 - rot];
}

void copy_tab(int **table, int **b_tab, int nr, int nc) {
    for(int i = 0; i < nr; i++) {
        for(int j = 0; j < nc; j++) {
            b_tab[nr*i+j][0] = table[nr*i+j][0];
            b_tab[nr*i+j][1] = table[nr*i+j][1];
        }
    }
}

void print_tab(int **table, int nr, int nc) {
    for(int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            printf("%d/%d ", table[nr * i + j][0], table[nr * i + j][1]);
        }
        printf("\n");
    }
}

void dealloca(tess *inp, int **table, int nr) {
    free(inp);
    for(int i = 0; i < nr; i++) {
        free(table[i]);
    }
    free(table);
}