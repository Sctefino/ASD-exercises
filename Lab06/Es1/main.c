#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int inizio,
    fine;
} att;

int riempi_struct(FILE *fin, int *N, att **v, att **a);
void MergeSort(att *v, att *a, int N);
//int max(int n1, int n2);
int durata(att *v, int i);
void dealloca(att *v, att *a);
void LISDP(att *val, int N);
void LISprint(att *val, int *P, int i);
//int LIS(att *val, int N);
//int LISR(att *val, int i);


int main() {
    FILE *fin = NULL;
    att *v, *a;
    int N;

    if(riempi_struct(fin, &N, &v, &a))
        return 1;

    MergeSort(v, a, N);

    for(int i = 0; i < N; i++) {
        printf("inizio: %d, fine: %d\n", v[i].inizio, v[i].fine);
    }

    LISDP(v, N);

    dealloca(v, a);

    return 0;
}

int riempi_struct(FILE *fin, int *N, att **v, att **a) {
    fin = fopen("att1.txt", "r");
    if(fin == NULL)
        return 1;

    fscanf(fin, "%d\n", N);
    *v = (att *) malloc(*N * sizeof(att));
    if(*v == NULL)
        return 1;

    *a = (att *) malloc(*N * sizeof(att));
    if(*a == NULL)
        return 1;

    for(int i = 0; i < *N; i++) {
        fscanf(fin, "%d %d\n", &((*v+i)->inizio), &((*v+i)->fine));
    }
    fclose(fin);

    return 0;
}

void Merge(att *v, att *a, int l, int q, int r) {
    int i, j, k;
    i = l;
    j = q + 1;
    for (k = l; k <= r; k++)
        if (i > q)
            a[k] = v[j++];
        else if (j > r)
            a[k] = v[i++];
        else if (v[i].fine <= v[j].fine)
            a[k] = v[i++];
        else
            a[k] = v[j++];
    for (k = l; k <= r; k++ )
        v[k] = a[k];
    return;
}

void MergeSortR(att *v, att *a, int l, int r) {
    int q;
    if (l >= r)
        return;
    q = (l + r)/2;
    MergeSortR(v, a, l, q);
    MergeSortR(v, a, q+1, r);
    Merge(v, a, l, q, r);
}

void MergeSort(att *v, att *a, int N) {
    int l=0, r=N-1;
    MergeSortR(v, a, l, r);
}

//int LIS(att *val, int N) {
//    int ris, ris_m = 0;
//
//    for(int i = 0; i < N; i++) {
//        ris = LISR(val, i);
//        ris_m = max(ris, ris_m);
//    }
//    return ris_m;
//}
//int LISR(att *val, int i) {
//    int j, ris;
//    if (i == 0)
//        return durata(val, i);
//    ris = durata(val, i);
//    for (j=0; j < i; j++)
//        if (val[j].fine <= val[i].inizio)
//            ris = max(ris, durata(val, i) + LISR(val, j));
//    return ris;
//}
//
//int max(int n1, int n2) {
//    if(n1 >= n2)
//        return n1;
//    return n2;
//}

int durata(att *v, int i) {
    return (v+i)->fine - (v+i)->inizio;
}

void dealloca(att *v, att *a) {
    free(v);
    free(a);
}

void LISDP(att *val, int N) {
    int ris = 1, L[N], P[N], last = 1;

    L[0] = durata(val, 0);
    P[0] = -1;

    for (int i = 1; i < N; i++) {
        L[i] = durata(val, i);
        P[i] = -1;
        for (int j = 0; j < i; j++)
            if ((val[j].fine <= val[i].inizio) && (L[i] < durata(val, i) + L[j])) {
                L[i] = durata(val, i) + L[j];
                P[i] = j;
            }
        if (ris < L[i]) {
            ris = L[i];
            last = i;
        }
    }
    printf("One of the Longest Increasing Sequences is\n");
    LISprint(val, P, last);
    printf("\nand its length is %d\n", ris);
}

void LISprint(att *val, int *P, int i) {
    if (P[i]==-1) {
        printf("(%d, %d) ", val[i].inizio, val[i].fine);
        return;
    }
    LISprint(val, P, P[i]);
    printf("(%d, %d) ", val[i].inizio, val[i].fine);
}