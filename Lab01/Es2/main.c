#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define NR 1001
#define LMAX 30

typedef struct {
    char tratta[LMAX], partenza[LMAX], destinazione[LMAX], data[LMAX], ora_partenza[LMAX], ora_arrivo[LMAX];
    int ritardo;
} corse;

typedef enum {
    r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine, r_err
} comando_e;

int riempi_struct(corse tratte[], FILE *fin);
comando_e leggiComando(char domanda[]);
void toLower(char stringa[]);
int selezionaDati(corse tratte[], int nr, int cmd, char domanda[]);
void date(corse tratte[], char data1[], char data2[], int nr);
void partenza(corse tratte[], char part[], int nr);
void capolinea(corse tratte[], char arr[], int nr);
void ritardo(corse tratte[], char data1[], char data2[], int nr);
void ritardo_tot(corse tratte[], char codice[], int nr);

int main() {
    int nr, cmd, continua = 0;
    char domanda[200];
    corse tratte[NR];
    FILE *fpin;

    fpin = fopen("corse.txt", "r");

    if(fpin == NULL) {
        printf("Errore in lettura file di input");
        return 1;
    }

    nr = riempi_struct(tratte, fpin);

    fclose(fpin);

    while(continua == 0) {
        printf("Inserire comando:");
        gets(domanda);
        cmd = leggiComando(domanda);
        continua = selezionaDati(tratte, nr, cmd, domanda);
    }

    return 0;
}

int riempi_struct(corse tratte[], FILE *fin) {
    int x = 0;
    if(!feof(fin)) {
        fscanf(fin, "%d", &x);
    }
    for(int i = 0; i < x; i++) {
        fscanf(fin, "%s %s %s %s %s %s %d", &tratte[i].tratta, tratte[i].partenza, tratte[i].destinazione,
               tratte[i].data, tratte[i].ora_partenza, tratte[i].ora_arrivo, &tratte[i].ritardo);
    }

    return x;
}

comando_e leggiComando(char domanda[]) {
    comando_e c;
    char cmd[LMAX];
    char table[r_err][LMAX] = {
            "date", "partenza", "capolinea", "ritardo", "ritardo_tot", "fine"
    };

    sscanf(domanda, "%s", cmd);
    toLower(cmd);

    c = r_date;
    while(c < r_err && strcmp(cmd, table[c]) != 0) {
        c++;
    }

    return c;
}

void toLower(char stringa[]) {

    for(int i = 0; i < strlen(stringa); i++) {
        if(isalpha(stringa[i]) == 1) {
            stringa[i] = tolower(stringa[i]);
        }
    }
}

int selezionaDati(corse tratte[], int nr, int cmd, char domanda[]) {
    char part[LMAX], arr[LMAX], data1[LMAX], data2[LMAX], code[LMAX];
    switch(cmd) {
        case r_date:
            sscanf(domanda, "%*s %s %s", data1, data2);
            date(tratte, data1, data2, nr);
            break;
        case r_partenza:
            sscanf(domanda, "%*s %s", part);
            partenza(tratte, part, nr);
            break;
        case r_capolinea:
            sscanf(domanda, "%*s %s", arr);
            capolinea(tratte, arr, nr);
            break;
        case r_ritardo:
            sscanf(domanda, "%*s %s %s", data1, data2);
            ritardo(tratte, data1, data2, nr);
            break;
        case r_ritardo_tot:
            sscanf(domanda, "%*s %s", code);
            ritardo_tot(tratte, code, nr);
            break;
        case r_fine:
            return 1;
        case r_err:
            printf("Comando non riconosciuto!\n");
            break;
    }

    return 0;
}

void date(corse tratte[], char data1[], char data2[], int nr) {
    int flag = 0;

    for(int i = 0; i < nr; i++) {
        //Se la data della tratta è nell'intervallo considerato, stampo la tratta
        if(strcmp(data1, tratte[i].data) < 0 && strcmp(data2, tratte[i].data) > 0) {
            printf("%s %s %s %s %s %s %d\n", tratte[i].tratta, tratte[i].partenza, tratte[i].destinazione,
                   tratte[i].data, tratte[i].ora_partenza, tratte[i].ora_arrivo, tratte[i].ritardo);
            flag = 1;
        }
    }

    if(flag == 0) {
        printf("Nessuna tratta nel periodo di tempo indicato!\n");
    }
}

void partenza(corse tratte[], char part[], int nr) {
    int flag = 0;

    for(int i = 0; i < nr; i++) {
        if(strcmp(tratte[i].partenza, part) == 0) {
            printf("%s %s %s %s %s %s %d\n", tratte[i].tratta, tratte[i].partenza, tratte[i].destinazione,
                   tratte[i].data, tratte[i].ora_partenza, tratte[i].ora_arrivo, tratte[i].ritardo);
            flag = 1;
        }
    }

    if(flag == 0) {
        printf("Nessuna tratta con partenza indicata!\n");
    }
}

void capolinea(corse tratte[], char arr[], int nr) {
    int flag = 0;

    for(int i = 0; i < nr; i++) {
        if(strcmp(tratte[i].destinazione, arr) == 0) {
            printf("%s %s %s %s %s %s %d\n", tratte[i].tratta, tratte[i].partenza, tratte[i].destinazione,
                   tratte[i].data, tratte[i].ora_partenza, tratte[i].ora_arrivo, tratte[i].ritardo);
            flag = 1;
        }
    }

    if(flag == 0) {
        printf("Nessuna tratta con destinazione indicata!\n");
    }
}

void ritardo(corse tratte[], char data1[], char data2[], int nr) {
    int flag = 0;

    for(int i = 0; i < nr; i++) {
        //Se la data della tratta è nell'intervallo considerato, e se il ritardo è non nullo, stampo la tratta
        if(strcmp(data1, tratte[i].data) < 0 && strcmp(data2, tratte[i].data) > 0 && tratte[i].ritardo != 0) {
            printf("%s %s %s %s %s %s %d\n", tratte[i].tratta, tratte[i].partenza, tratte[i].destinazione,
                   tratte[i].data, tratte[i].ora_partenza, tratte[i].ora_arrivo, tratte[i].ritardo);
            flag = 1;
        }
    }

    if(flag == 0) {
        printf("Nessuna tratta con ritardo nel periodo di tempo indicato!\n");
    }
}

void ritardo_tot(corse tratte[], char codice[], int nr) {
    int rit_tot = 0, flag = 0;

    for(int i = 0; i < nr; i++) {
        if(strcmp(tratte[i].tratta, codice) == 0) {
            rit_tot += tratte[i].ritardo;
            flag = 1;
        }
    }
    if(flag == 1) {
        printf("Il ritardo complessivo accumulato dalla tratta %s e': %d minuti\n", codice, rit_tot);
    }
    else {
        printf("Nessuna tratta col codice indicato trovata!\n");
    }
}