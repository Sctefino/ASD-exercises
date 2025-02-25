#include "equipArray.h"
#define LENGTH 100

struct equipArray_s {
    int inUso,
        vettEq[EQUIP_SLOT];
};

equipArray_t equipArray_init() {
    equipArray_t equipArray = malloc(sizeof(struct equipArray_s));
    equipArray->inUso = 0;
    for(int i = 0; i < EQUIP_SLOT; i++) {
        equipArray->vettEq[i] = -1;
    }
    return equipArray;
}

void equipArray_free(equipArray_t equipArray) {
    free(equipArray);
}

int equipArray_inUse(equipArray_t equipArray) {
    return equipArray->inUso;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray) {
    int i = 0;
    while(i < equipArray->inUso) {
        if(equipArray->vettEq[i] != -1) {
            invArray_printByIndex(fp, invArray, equipArray->vettEq[i]);
            i++;
        }
    }
}

void equipArray_update(equipArray_t equipArray, invArray_t invArray) {
    char name[LENGTH];
    static int index, choice, i;

    printf("Aggiungere (0) o rimuovere (1)?");
    scanf("%d", &choice);

    if(choice && equipArray_inUse(equipArray) == 0) {
        printf("Inventario vuoto!\n");
        return;
    }
    else if(!choice && equipArray_inUse(equipArray) == EQUIP_SLOT) {
        printf("Inventario pieno!\n");
        return;
    }

    printf("Inserire nome oggetto:");
    scanf("%s", name);
    index = invArray_searchByName(invArray, name);

    if(index == -1) {
        printf("Oggetto non trovato!");
        return;
    }

    if(!choice) {
        for(i = 0; equipArray->vettEq[i] != -1; i++);
        equipArray->vettEq[i] = index;
        equipArray->inUso++;
    }
    else {
        for(i = 0; i < EQUIP_SLOT; i++) {
            if(equipArray->vettEq[i] == index) {
                equipArray->vettEq[i] = -1;
                equipArray->inUso--;
                break;
            }
        }
    }

}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
    return equipArray->vettEq[index];
}