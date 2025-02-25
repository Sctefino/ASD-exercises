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
    ord_data, ord_tratta, ord_partenza, ord_arrivo, ric_partenza, r_fine, stampa, err
} comando_e;

int riempi_struct(corse tratte[], FILE *fin);
comando_e leggiComando(char domanda[]);
void toLower(char stringa[]);
int selezionaDati(corse tratte[], int nr, int cmd, char domanda[], corse **p);
void print(corse tratte[], int nr);
void print_p(corse *p[], int nr);
void sort_part(corse *p[], int nr);

int main() {
    int nr, fine = 0;
    char domanda[100];
    corse tratte[NR], *p[6][NR];
    FILE *fpin;
    comando_e cmd;

    fpin = fopen("corse.txt", "r");

    if(fpin == NULL) {
        printf("Errore in lettura file di input");
        return 1;
    }

    nr = riempi_struct(tratte, fpin);

    fclose(fpin);
    while(!fine) {
        printf("Inserire comando:\n'stampa': stampa i contenuti del file\n'data': "
               "ordina le tratte per data \n'tratta': ordina le tratte per codice\n'partenza': ordina le tratte per "
               "stazione di partenza\n'arrivo': ordina le tratte per stazione di arrivo\n'ricerca': ricerca le tratte con stazione di partenza"
               " (anche parzialmente) indicata\n'fine': uscita dal programma\n");
        gets(domanda);

        cmd = leggiComando(domanda);
        fine = selezionaDati(tratte, nr, cmd, domanda, p[cmd]);
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
    char table[err][LMAX] = {
            "data", "tratta", "partenza", "arrivo", "ricerca", "fine", "stampa"
    };

    sscanf(domanda, "%s", cmd);
    toLower(cmd);

    c = ord_data;
    while(c < err && strcmp(cmd, table[c]) != 0) {
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

int selezionaDati(corse tratte[], int nr, int cmd, char domanda[], corse **p) {
    corse *temp;
    int i = 0, j, r = nr - 1, m, found = 0;
    char part[LMAX];

    for(int a = 0; a < nr; a++) {
        p[a] = &tratte[a];
    }

    switch(cmd) {
        case stampa:
            print(tratte, nr);
            break;
        case ord_data:
            for (i = 1; i <= r; i++) {
                temp = p[i];
                j = i - 1;
                while (j >= 0 && (strcmp(temp->data,p[j]->data) < 0 ||
                        (strcmp(temp->data, p[j]->data) == 0 && strcmp(temp->ora_partenza, p[j]->ora_partenza) < 0))) {
                    p[j+1] = p[j];
                    j--;
                }
                p[j+1] = temp;
            }
            print_p(p, nr);
            break;
        case ord_tratta:
            for (i = 1; i <= r; i++) {
                temp = p[i];
                j = i - 1;
                while (j >= 0 && strcmp(temp->tratta,p[j]->tratta) < 0){
                    p[j+1] = p[j];
                    j--;
                }
                p[j+1] = temp;
            }

            print_p(p, nr);

            break;
        case ord_partenza:
            sort_part(p, nr);
            print_p(p, nr);

            break;
        case ord_arrivo:
            for (i = 1; i <= r; i++) {
                temp = p[i];
                j = i - 1;
                while (j >= 0 && strcmp(temp->destinazione,p[j]->destinazione) < 0){
                    p[j+1] = p[j];
                    j--;
                }
                p[j+1] = temp;
            }
            print_p(p, nr);

            break;
        case ric_partenza:
            sscanf(domanda ,"%*s %s", part);
            //ricerca lineare
            printf("Ricerca lineare:\n");
            for(int z = 0; z < nr; z++) {
                if(strncmp(tratte[z].partenza, part, strlen(part)) == 0) {
                    printf("%s %s %s %s %s %s %d\n", tratte[z].tratta, tratte[z].partenza, tratte[z].destinazione,
                           tratte[z].data, tratte[z].ora_partenza, tratte[z].ora_arrivo, tratte[z].ritardo);
                }
            }
            //ricerca dicotomica
            printf("Ricerca dicotomica:\n");
            sort_part(p, nr);
            while(i <= r && !found) {
                m = (i + r)/2;
                if(strncmp(p[m]->partenza, part, strlen(part)) == 0) {
                    found = 1;
                }
                else if (strncmp(p[m]->partenza, part, strlen(part)) < 0) {
                    i = m+1;
                }
                else {
                    r = m-1;
                }
            }
            for(int a = m - 1, b = m; (a >= 0 && strncmp(p[a]->partenza, part, strlen(part)) == 0) || (b <= r &&
                                        strncmp(p[b]->partenza, part, strlen(part)) == 0); a--, b++) {
                if(a >= 0 && strncmp(p[a]->partenza, part, strlen(part)) == 0)
                    printf("%s %s %s %s %s %s %d\n", p[a]->tratta, p[a]->partenza, p[a]->destinazione,
                           p[a]->data, p[a]->ora_partenza, p[a]->ora_arrivo, p[a]->ritardo);
                if(b <= r && strncmp(p[b]->partenza, part, strlen(part)) == 0)
                    printf("%s %s %s %s %s %s %d\n", p[b]->tratta, p[b]->partenza, p[b]->destinazione,
                           p[b]->data, p[b]->ora_partenza, p[b]->ora_arrivo, p[b]->ritardo);
            }

            break;
        case r_fine:
            return 1;
        case err:
            printf("Comando non riconosciuto!\n");
            break;
    }
    return 0;
}

void print(corse tratte[], int nr) {
    for(int i = 0; i < nr; i++) {
        printf("%s %s %s %s %s %s %d\n", tratte[i].tratta, tratte[i].partenza, tratte[i].destinazione,
               tratte[i].data, tratte[i].ora_partenza, tratte[i].ora_arrivo, tratte[i].ritardo);
    }
}

void print_p(corse *p[], int nr) {
    for(int i = 0; i < nr; i++) {
        printf("%s %s %s %s %s %s %d\n", p[i]->tratta, p[i]->partenza, p[i]->destinazione,
               p[i]->data, p[i]->ora_partenza, p[i]->ora_arrivo, p[i]->ritardo);
    }
}

void sort_part(corse *p[], int nr) {
    int i, j;
    corse *temp;

    for (i = 1; i <= nr-1; i++) {
        temp = p[i];
        j = i - 1;
        while (j >= 0 && strcmp(temp->partenza,p[j]->partenza) < 0){
            p[j+1] = p[j];
            j--;
        }
        p[j+1] = temp;
    }
}
