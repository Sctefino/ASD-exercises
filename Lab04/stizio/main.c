#include <stdio.h>
#include <stdlib.h>

void coll_rec(int max, int *bmax, int *sol, int *bsol, int *val, int pos, int *p, int pos1, int cnt, int *flag);
void check_pos(int pos, int *p);
void malloc2dP(int ***x, int nr);
void riempi_struct(FILE *fin, int **x, int nr);
char converti(int x);

int main() {
    FILE *fin;
    int **x, nr, max = 0, sum, cnt = 0, *sol, *b_sol, pos = 0, p[2], flag = 0, l = 0, r, m;
    fin = fopen("easy_test_set.txt", "r");
    fscanf(fin, "%d\n", &nr);

    malloc2dP(&x, nr);

    riempi_struct(fin, x, nr);


    for(int i = 0; i < nr; i++) {

        sum = x[i][0] + x[i][1] + x[i][2] + x[i][3];
        r = sum;

        sol = (int *) malloc(sum * sizeof(int));
        b_sol = (int *) malloc(sum * sizeof(int));
        while(l < r) {
            m = (l + r) / 2;
            flag = 0;
            for (int j = 0; j < 4 && !flag; j++) {
                coll_rec(m, &max, sol, b_sol, x[i], j, p, pos, cnt, &flag);
            }
            if(flag)
                l = m + 1;
            else
                r = l - 1; //se non ho trovato una collana con la lunghezza prestabilita, forzo l'uscita, perché la funzione
                            // mi ha comunque salvato la collana di lunghezza massima
        }
        printf("TEST #%d:\n"
               "zaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n"
               "Collana massima di lunghezza %d\n"
               "Collana di lunghezza max:", i+1, x[i][0], x[i][1], x[i][2], x[i][3], sum, max);

        for(int s = 0; s < max; s++)
            printf(" %c", converti(b_sol[s]));
        printf("\n");
        l = 0;
        flag = 0;
        max = 0;
        free(sol);
        free(b_sol);
    }

    fclose(fin);
    return 0;
}


void malloc2dP(int ***x, int nr) {
    *x = (int **) malloc(nr * sizeof(int *));
    for(int i = 0; i < nr; i++) {
        (*x)[i] = (int *) malloc(4 * sizeof(int));
    }
}

void riempi_struct(FILE *fin, int **x, int nr) {
    for(int i = 0; i < nr; i++) {
        fscanf(fin, "%d %d %d %d\n", &x[i][0], &x[i][1], &x[i][2], &x[i][3]);
    }
}

void coll_rec(int max, int *bmax, int *sol, int *bsol, int *val, int gem, int *p, int pos, int cnt, int *flag) {
    if(cnt >= max || val[gem] == 0) {
        if(cnt > *bmax) {
            *bmax = cnt;
            for(int i = 0; i < cnt; i++) {
                bsol[i] = sol[i];
            }
        }
        if(cnt == max)
            *flag = 1;
        return;
    }

    if(*flag)
        return;
    val[gem]--;
    sol[pos] = gem;
    cnt++;

    check_pos(gem, p);
    coll_rec(max, bmax, sol, bsol, val, p[0], p, pos + 1, cnt, flag);
    coll_rec(max, bmax, sol, bsol, val, p[1], p, pos + 1, cnt, flag);
    val[gem]++;
}

void check_pos(int pos, int *p) {
    if(pos % 2 == 0) {
        p[0] = 0;
        p[1] = 1;
    }
    else {
        p[0] = 3;
        p[1] = 2;
    }
}

char converti(int x) {
    if(x == 0)
        return 'z';
    if(x == 1)
        return 'r';
    if(x == 2)
        return 't';
    return 's';
}