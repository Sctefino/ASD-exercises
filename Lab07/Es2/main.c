#include <stdio.h>
#include <stdlib.h>

#define LMAX 100
#define DIAGMAX 5
#define NUMDIAG 3

typedef struct {
    char name[LMAX];
    int tipo,
        ingr,
        usc,
        prec,
        fin;
    float val;
    int diff;
} elemento;

typedef struct {
    elemento **diag;
    int *dims;
    int n;
} diagonali;

void riempi_elementi(elemento **el, int *n, FILE *fp);
void disp_rip_wrap(elemento *el, diagonali diags, int n, int DD, int DP);
int check_el_acr(elemento *sol, int cnt);
float count_points(diagonali diags, int sol[NUMDIAG]);
int check(diagonali diags, int sol[NUMDIAG]);
int somma_diff(diagonali diags, int cnt);
void dealloca(diagonali diags, int n);


int main(int argc, char **argv) {
    FILE *fp = NULL;
    elemento *el = NULL;
    diagonali diags;
    int n = 0, DD, DP;

    riempi_elementi(&el, &n, fp);

    for(int i = 1; i < argc; i+=2) {
        DD = atoi(argv[i]);
        DP = atoi(argv[i+1]);

        disp_rip_wrap(el, diags, n, DD, DP);
    }

    free(el);

    return 0;
}

void riempi_elementi(elemento **el, int *n, FILE *fp) {

    fp = fopen("elementi.txt", "r");

    fscanf(fp, "%d", n);

    *el = malloc(*n * sizeof(elemento));

    for(int i = 0; i < *n; i++) {
        fscanf(fp, "%s %d %d %d %d %d %f %d", (*el+i)->name, &((*el+i)->tipo), &((*el+i)->ingr), &((*el+i)->usc),
               &((*el+i)->prec), &((*el+i)->fin), &((*el+i)->val), &((*el+i)->diff));
    }

    fclose(fp);
}

int check_el_acr(elemento *sol, int cnt) {
    for(int i = 0; i < cnt; i++) {
        if(sol[i].tipo == 1 || sol[i].tipo == 2)
            return 1;
    }
    return 0;
}

void save_diag(elemento *source, elemento **dest, int n) {
    *dest = malloc(n*sizeof(elemento));
    for(int i = 0; i < n; i++) {
        (*dest)[i] = source[i];
    }
}

float count_points(diagonali diags, int sol[NUMDIAG]) {
    float x = 0, y = 0;

    for(int i = 0; i < NUMDIAG; i++) {
        for(int j = 0; j < diags.dims[sol[i]]; j++) {
            y += diags.diag[sol[i]][j].val;
        }
        if(i == NUMDIAG - 1 && diags.diag[sol[i]][diags.dims[sol[i]]-1].diff >= 8)
            y = y * 1.5;
        x += y;
        y = 0;
    }

    return x;
}


void disp_rip_R(elemento *el, elemento *sol, diagonali *diags, int *pos, int n, int cnt, int sum, int max, int DD) {

    if (cnt >= max || sol[cnt-1].fin == 1) {
        if(check_el_acr(sol, cnt)) {
            if(*pos >= diags->n) {
                diags->n = diags->n * 2;
                diags->diag = realloc(diags->diag, diags->n * sizeof(elemento *));
                diags->dims = realloc(diags->dims, diags->n * sizeof(int));
            }
            save_diag(sol, (diags->diag) + *pos, cnt); //salvo la diagonale valida nel data type diags
            diags->dims[*pos] = cnt;
            (*pos)++;
        }
        return;
    }

    for(int i = 0; i < n; i++) {
        while(i >= n || (cnt == 0 && ((el+i)->prec == 1 || (el+i)->ingr == 0)) || (cnt != 0 && (sol+cnt-1)->usc != (el+i)->ingr) ||  //effettuo pruning skippando tutti gli elementi non conformi alle regole
                sum + (el+i)->diff > DD) {
            if (i >= n) //non ho trovato altri elementi validi
                return;
            i++;
        }
        sol[cnt] = el[i];
        disp_rip_R(el, sol, diags, pos, n, cnt+1,sum + (el+i)->diff, max, DD);
    }

}

void disp_rip_R2(int sol[NUMDIAG], int bsol[NUMDIAG], diagonali diags, int n, int DP, int cnt, int sum, float *bp, float p) {
    if(cnt >= NUMDIAG) {
        if (check(diags, sol)) { // se valida anche per le ultime regole da seguire, che si basano sull'insieme delle diagonali e non sulla singola, valuto il punteggio
            p = count_points(diags, sol);
            if(p > *bp) {
                for(int i = 0; i < NUMDIAG; i++) {
                    bsol[i] = sol[i];
                }
                *bp = p;
            }
        }
        return;
    }

    for(int i = 0; i < n; i++) {
        while(i >= n || sum + somma_diff(diags, i) > DP) {
            if (i >= n) //non ho trovato altri elementi validi
                return;
            i++;
        }
        sol[cnt] = i;
        disp_rip_R2(sol, bsol, diags, n, DP, cnt+1, sum + somma_diff(diags, i), bp, p);
    }
}

void disp_rip_wrap(elemento *el, diagonali diags, int n, int DD, int DP) {
    int cnt = 0, sum = 0, sum2 = 0, pos = 0, cnt2 = 0, solu[NUMDIAG] = {0}, bsol[NUMDIAG] = {0};
    float bp = 0, p = 0;

    elemento *sol = malloc(DIAGMAX * sizeof(elemento));

    diags.diag = malloc(sizeof(elemento *));
    diags.dims = malloc(sizeof(int));
    diags.n = 1;

    for(int i = 0; i <= DIAGMAX; i++) //ricorsione per salvare tutte le diagonali valide (fino a questo punto) in una matrice di tipi elemento
        disp_rip_R(el, sol, &diags, &pos, n, cnt, sum, i, DD);

    disp_rip_R2(solu, bsol, diags, pos, DP, cnt2, sum2, &bp, p); //ricorsione per selezionare la disposizione migliore, tra le diagonali valide

    for(int i = 0; i < NUMDIAG; i++) {
        printf("DIAG %d:\n", i+1);
        for(int j = 0; j < diags.dims[bsol[i]]; j++) {
            printf("%s ", diags.diag[bsol[i]][j].name);
        }
        printf("\n");
    }
    printf("\n");

    dealloca(diags, pos);
    free(sol);
}

int check(diagonali diags, int sol[NUMDIAG]) { //controllo di presenza di due elementi acrobatici in fila, un acrobatico avanti e uno indietro in tutto il programma
    int flag_f = 0, flag_d = 0, flag_2row = 0;
    for(int i = 0; i < NUMDIAG; i++) {
        for(int j = 0; j < diags.dims[sol[i]]; j++) {
            if(diags.diag[sol[i]][j].tipo == 1) {
                flag_f = 1;
                flag_2row++;
            }
            else if(diags.diag[sol[i]][j].tipo == 2) {
                flag_d = 1;
                flag_2row++;
            }
            else if(flag_2row < 2)
                flag_2row = 0;
        }
    }

    return (flag_d == 1 && flag_f == 1 && flag_2row >= 2);
}

int somma_diff(diagonali diags, int cnt) { //calcolo la somma della difficoltà degli esercizi di una diagonale
    int x = 0;
    for(int j = 0; j < diags.dims[cnt]; j++)
        x += diags.diag[cnt][j].diff;

    return x;
}

void dealloca(diagonali diags, int n) {
    for(int i = 0; i < n; i++) {
        free(diags.diag[i]);
    }
    free(diags.diag);
    free(diags.dims);
}