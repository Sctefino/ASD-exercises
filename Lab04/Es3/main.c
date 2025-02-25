#include <stdio.h>
#include <stdlib.h>

//typedef struct {
//    int primo,
//        secondo;
//} poss_gem;
void coll_rec(int max, int *bmax, int *sol, int *bsol, int *val, int pos, int *p, int pos1, int cnt);
void check_pos(int pos, int *p);
void malloc2dP(int ***x, int nr);
void riempi_struct(FILE *fin, int **x, int nr);
char converti(int x);

int main() {
    FILE *fin;
    int **x, nr, max_loc = 0, max = 0, sum, cnt = 0, *sol, *bsol_loc, *b_sol, pos = 0, p[2];
    //poss_gem v[4];

//    for(int i = 0; i < 4; i++) {
//        if(i % 2 == 0) {
//            v[i].primo = 0;
//            v[i].secondo = 1;
//        }
//        else {
//            v[i].primo = 2;
//            v[i].secondo = 3;
//        }
//    }
    fin = fopen("easy_test_set.txt", "r");
    fscanf(fin, "%d\n", &nr);

    malloc2dP(&x, nr);

    riempi_struct(fin, x, nr);


    for(int i = 0; i < nr; i++) {

        sum = x[i][0] + x[i][1] + x[i][2] + x[i][3];

        sol = (int *) malloc(sum * sizeof(int));
        bsol_loc = (int *) malloc(sum * sizeof(int));

        for (int j = 0; j < 4; j++) {
            coll_rec(sum, &max_loc, sol, bsol_loc, x[i], j, p, pos, cnt);
            if(max_loc > max) {
                max = max_loc;
                b_sol = (int *) malloc(max * sizeof(int));
                for(int k = 0; k < max; k++) {
                    b_sol[k] = bsol_loc[k];
                }
            }
        }
        printf("TEST #%d:\n"
               "zaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n"
               "Collana massima di lunghezza %d\n"
               "Collana di lunghezza max:", i+1, x[i][0], x[i][1], x[i][2], x[i][3], sum, max);

        for(int s = 0; s < max; s++)
            printf(" %c", converti(b_sol[s]));
        printf("\n");
        max = 0;
        max_loc = 0;
        free(sol);
        free(bsol_loc);
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

void coll_rec(int max, int *bmax, int *sol, int *bsol, int *val, int gem, int *p, int pos, int cnt) {
    if(cnt >= max || val[gem] == 0) {
        if(cnt > *bmax) {
            *bmax = cnt;
            for(int i = 0; i < cnt; i++) {
                bsol[i] = sol[i];
            }
        }
        return;
    }

    val[gem]--;
    sol[pos] = gem;
    cnt++;

    check_pos(gem, p);
    coll_rec(max, bmax, sol, bsol, val, p[0], p, pos + 1, cnt);
    coll_rec(max, bmax, sol, bsol, val, p[1], p, pos + 1, cnt);
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

//void coll_rec(int max, int *bmax, int *sol, int *bsol, int *val, int gem, int *p, int pos, int cnt, int *flag) {
//    if(cnt >= max || val[gem] == 0) {
//        if(cnt > *bmax) {
//            *bmax = cnt;
//            for(int i = 0; i < cnt; i++) {
//                bsol[i] = sol[i];
//            }
//        }
//        if(cnt == max)
//            *flag = 1;
//        return;
//    }
//
//    if(*flag)
//        return;
//    val[gem]--;
//    sol[pos] = gem;
//    cnt++;
//
//
//    coll_rec(max, bmax, sol, bsol, val, p[gem % 2], p, pos + 1, cnt, flag);
//    coll_rec(max, bmax, sol, bsol, val, p[gem % 2] + 1, p, pos + 1, cnt, flag);
//    val[gem]++;
//}