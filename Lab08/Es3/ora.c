#include "ora.h"
#include <stdio.h>

ora getora(int o, int min) {
    ora h;

    h.ore = o;
    h.minuti = min;

    return h;
}

void orastore(ora hour) {
    printf("%d:%d ", hour.ore, hour.minuti);
}