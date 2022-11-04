#include <stdio.h>

int majority(int *a, int N);
int check_maj(int b, int *a, int N);

int main() {
    int v[20] = { 3, 3, 9, 4, 3, 5, 3}, N = 7, maj;

    maj = majority(v, N);

    if(maj != -1)
        printf("L'elemento maggioritario del vettore e': %d", maj);
    else
        printf("Non esiste l'elemento maggioritario nel vettore!");

    return 0;
}

int majority(int *a, int N) {
    int x, y, m;
    if(N == 1)
        return *a;

    m = (N-1)/2 + 1;
    x = majority(a, m);
    y = majority(a+m, N-m);
    if(x == y)
        return x;
    x = check_maj(x, a, N);
    y = check_maj(y, a, N);
    if(x != -1)
        return x;
    if(y != -1)
        return y;
    return -1;
}

int check_maj(int b, int *a, int N) {
    int count = 0;

    for(int i = 0; i < N; i++) {
        if(a[i] == b)
            count++;
    }

    if(count > N/2)
        return b;

    return -1;
}