#include <stdio.h>
#include <stdlib.h>

int **malloc2dR(int nr, int nc);
void malloc2dP(int nr, int nc, int ***p_matr);
void separa(int **mat, int nr, int nc, int **v1, int **v2);
void free2d(int **mat, int nr);

int main() {
    int nr, nc, **matr, choice = 0, *v1, *v2;
    FILE *fp;

    fp = fopen("mat.txt", "r");
    if(fp == NULL) {
        return -1;
    }

    fscanf(fp, "%d %d", &nr, &nc);
    printf("Funzioine by value (1) o by reference (2)?\n");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            matr = malloc2dR(nr, nc);

            for(int i = 0; i < nr; i++) {
                for(int j = 0; j < nc; j++) {
                    fscanf(fp, "%d", &matr[i][j]);
                }
            }
            break;
        case 2:
            malloc2dP(nr, nc, &matr);
            for(int i = 0; i < nr; i++) {
                for(int j = 0; j < nc; j++) {
                    fscanf(fp, "%d", &matr[i][j]);
                }
            }
            break;
        default:
            printf("Scelta non congrua con le opzioni fornite!");
            exit(-1);
    }

    fclose(fp);

    separa(matr, nr, nc, &v1, &v2);

    printf("Caselle nere:\n");
    for(int i = 0; (i < (nr * nc / 2) + 1 && nr * nc % 2 != 0) || i < (nr * nc / 2); i++) {
        printf("%d ", v1[i]);
    }
    printf("\nCaselle bianche:\n");
    for(int i = 0; i < (nr * nc / 2); i++) {
        printf("%d ", v2[i]);
    }

    free2d(matr, nr);
    free(v1);
    free(v2);

    return 0;
}

int **malloc2dR(int nr, int nc) {
    int **m;

    m = (int **)malloc(nr*sizeof(int *));
    for(int i = 0; i < nr; i++) {
        m[i] = (int *)malloc(nc*sizeof(int));
    }
    return m;
}

void malloc2dP(int nr, int nc, int ***p_matr) {
    int **m;

    m = (int **)malloc(nr*sizeof(int *));
    for(int i = 0; i < nr; i++) {
        m[i] = (int *)malloc(nc*sizeof(int));
    }

    *p_matr = m;
}

void separa(int **mat, int nr, int nc, int **v1, int **v2) {
    int a = 0, b = 0;

    *v1 = (int *)malloc(nr*nc*sizeof(int)/2 + sizeof(int)); //in caso sia dispari, al primo vettore serve un numero in più
                                                                //e trattandosi di un solo numero, è conveniente semplicemente
                                                                //allocare 4 byte di memoria in più
    *v2 = (int *)malloc(nr*nc*sizeof(int)/2);

    for(int i = 0; i < nr; i++) {
        for(int j = 0; j < nc; j++) {
            if((i + j) % 2 == 0) {
                (*v1)[a++] = mat[i][j];
            }
            else {
                (*v2)[b++] = mat[i][j];
            }
        }
    }
}

void free2d(int **mat, int nr) {

    for(int i = 0; i < nr; i++) {
        free(mat[i]);
    }
    free(mat);
}