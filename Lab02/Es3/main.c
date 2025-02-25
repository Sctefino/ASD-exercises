#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define LMAX 30

typedef struct {
    char *tratta, *partenza, *destinazione, *data, *ora_partenza, *ora_arrivo;
    int ritardo;
} corse;

typedef enum {
    ord_data, ord_tratta, ord_partenza, ord_arrivo, ric_partenza, r_fine, stampa, file, err
} comando_e;

void riempi_struct(corse tratte[], FILE *fin, int nr);
comando_e leggiComando(char domanda[]);
void toLower(char stringa[]);
int selezionaDati(corse tratte[], int *nr, int cmd, char domanda[], corse **p, FILE *fin);
void print(corse tratte[], int nr);
void print_p(corse *p[], int nr);
void sort_part(corse *p[], int nr);
void malloc2dP(corse ****mp, int nr, int nc);
void free2dP(corse ***mp, int nr);
void alloc(char **s, FILE *fp, int *count);

int main() {
    int nr, fine = 0, temp;
    char domanda[100], filename[100];
    corse *tratte, ***p;
    FILE *fpin;
    comando_e cmd;

    fpin = fopen("corse.txt", "r");

    if(fpin == NULL) {
        printf("Errore in lettura file di input");
        return 1;
    }
    fscanf(fpin, "%d\n", &nr);
    temp = nr;
    tratte = (corse *) malloc(nr*sizeof(corse));
    if(tratte == NULL) {
        return 2;
    }
    riempi_struct(tratte, fpin, nr);

    malloc2dP(&p, err + 1, nr);

    while(!fine) {
        printf("Inserire comando:\n'file': cambio del file da cui prelevare i dati (se non usato come primo "
               "comando, verra' usato il file corse.txt)\n'stampa': stampa i contenuti del file\n'data': "
               "ordina le tratte per data \n'tratta': ordina le tratte per codice\n'partenza': ordina le tratte per "
               "stazione di partenza\n'arrivo': ordina le tratte per stazione di arrivo\n'ricerca': ricerca le tratte con stazione di partenza"
               " (anche parzialmente) indicata\n'fine': uscita dal programma\n");
        gets(domanda);

        cmd = leggiComando(domanda);
        if(cmd == file) { //libero e rialloco il vettore di puntatori a struct contenenti gli ordinamenti direttamente nel main

            fclose(fpin);
            sscanf(domanda, "%*s %s", filename);
            fpin = fopen(filename, "r");
            if(fpin == NULL) {
                return 1;
            }
            fscanf(fpin, "%d\n", &nr);

            free(tratte);

            tratte = (corse *) malloc(nr*sizeof(corse));
            if(tratte == NULL) {
                return 2;
            }

            riempi_struct(tratte, fpin, nr);

            free2dP(p, err + 1);
            malloc2dP(&p, err + 1, nr);

        }
        else
            fine = selezionaDati(tratte, &nr, cmd, domanda, p[cmd], fpin);
    }

    free(tratte);
    free2dP(p, err + 1);
    fclose(fpin);

    return 0;
}

void riempi_struct(corse tratte[], FILE *fin, int nr) {
    int *c, x = 0;
    c = &x;
    for(int i = 0; i < nr; i++) {
        alloc(&tratte[i].tratta, fin, c);
        alloc(&tratte[i].partenza, fin, c);
        alloc(&tratte[i].destinazione, fin, c);
        alloc(&tratte[i].data, fin, c);
        alloc(&tratte[i].ora_partenza, fin, c);
        alloc(&tratte[i].ora_arrivo, fin, c);
        fscanf(fin, "%d\n", &tratte[i].ritardo);
    }
}

comando_e leggiComando(char domanda[]) {
    comando_e c;
    char cmd[LMAX];
    char *table[err] = {
            "data", "tratta", "partenza", "arrivo", "ricerca", "fine", "stampa", "file"
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

int selezionaDati(corse tratte[], int *nr, int cmd, char domanda[], corse **p, FILE *fin) {
    corse *temp;
    int i = 0, j, r = *nr - 1, m, found = 0;
    char part[LMAX], filename[100];

    for(int a = 0; a < *nr; a++) {
        p[a] = &tratte[a];
    }

    switch(cmd) {
        case stampa:
            print(tratte, *nr);
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
            print_p(p, *nr);
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

            print_p(p, *nr);

            break;
        case ord_partenza:
            sort_part(p, *nr);
            print_p(p, *nr);

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
            print_p(p, *nr);

            break;
        case ric_partenza:
            sscanf(domanda ,"%*s %s", part);
            //ricerca lineare
            printf("Ricerca lineare:\n");
            for(int z = 0; z < *nr; z++) {
                if(strncmp(tratte[z].partenza, part, strlen(part)) == 0) {
                    printf("%s %s %s %s %s %s %d\n", tratte[z].tratta, tratte[z].partenza, tratte[z].destinazione,
                           tratte[z].data, tratte[z].ora_partenza, tratte[z].ora_arrivo, tratte[z].ritardo);
                }
            }
            //ricerca dicotomica
            printf("Ricerca dicotomica:\n");
            sort_part(p, *nr);
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
        default:
            return 1;
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

void malloc2dP(corse ****mp, int nr, int nc) {
    corse ***m;
    int i;
    m = (corse ***)malloc (nr*sizeof(corse **));
    for (i=0; i<nr; i++)
        m[i] = (corse **)malloc (nc*sizeof(corse *));
    *mp = m;
}

void free2dP(corse ***mp, int nr) {
    for(int i = 0; i < nr; i++) {
        free(mp[i]);
    }
    free(mp);
}

void alloc(char **s, FILE *fp, int *count) {
    char p;
    if(*count == 0) {
        *s = (char *) malloc(sizeof(char));
    }
    fscanf(fp, "%c", &p);
    if(p == ' ' || p == '\n'){
        return;
    }
    *(*s + *count) = p;
    (*count)++;
    *s = (char *) realloc(*s, (*count + 1) * sizeof(char));
    alloc(s, fp, count);
    if(*count != 0) {
        *(*s + *count) = '\0';
        *count = 0;
    }
    return;
}