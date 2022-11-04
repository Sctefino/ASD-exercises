#include <stdio.h>

int gcd(int a, int b);

int main() {
    int x = 24, y = 48, div;

    div = gcd(x,y);

    printf("%d", div);

    return 0;
}

int gcd(int a, int b) {
    int temp;

    if(a == 0 || a == b)
        return b;
    else if(b == 0) {
        return a;
    }

    if(a <= b) {
        temp = b;
        b = a;
        a = temp;
    }

    if(a % 2 == 0 && b % 2 == 0) {
        a = 2 * gcd(a/2, b/2);
    }
    else if(a % 2 != 0 && b % 2 != 0) {
        a = gcd((a-b)/2, b);
    }
    else if(a % 2 == 0) {
        a = gcd(a/2, b);
    }
    else {
        a = gcd(a, b/2);
    }

    return a;
}