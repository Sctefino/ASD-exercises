#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define LMAX 51

///VALUTA SE TOGLIERE GREATER E LASCIARE SOLO GEQ

typedef struct {
    char codice[6],
    nome[LMAX],
    cognome[LMAX],
    data_di_nascita[11],
    via[LMAX],
    citta[LMAX],
    cap[6]; //nonostante sia un intero, lo memorizzo come stringa per evitare la perdita delle prime due cifre in caso di cap cominciante con due zeri (es Roma)
} Item;

typedef struct node *link;
struct node {
    Item val;
    link next;
};

typedef enum {
    acq_tast, acq_file, ricerca, canc_codice, canc_date, stampa, fine, err
} comando_e;


comando_e leggiComando(char domanda[]);
void toLower(char stringa[]);
void riempi_struct(Item *temp, char *dati);
int *KeyGet(Item valore, int *data);
int KeyGreater(int *data1, int *data2);
link SortListIns(link h, Item valore);
int KeyEq(char *cod1, char *cod2);
int KeyGeq(char *data1, char *data2);
Item ITEMsetvoid(void);
Item SortListSearch(link h, char *k);
void stamp(Item persona, FILE *fout);
Item SortListDel_data(link *h, char *data1, char *data2);
Item SortListDel_cod(link *h, char *k);
void dealloca(link head);

int main() {
    FILE *fin = NULL, *fout = NULL;
    link head = NULL;
    char domanda[LMAX], dati[LMAX], filename[LMAX], cod[6], data1[11], data2[11];
    int end = 1, flag = 0;
    comando_e com;
    Item temp;

    while(end) {
        printf("Inserire comando:"
               "\n'acq_tast': acquisisce contenuti da inserire in lista tramite tastiera"
               "\n'acq_file': acquisisce contenuti da inserire in lista tramite file"
               "\n'ric_cod': ricerca elemento in lista per codice"
               "\n'canc_cod': estrae e cancella un elemento dalla lista di cui e' noto il codice"
               "\n'canc_date': estrae e cancella elementi dalla lista che appartengono a un intervallo di date"
               "\n'stampa': stampa su file la lista"
               "\n'fine': esce dal programma\n");
        gets(domanda);
        com = leggiComando(domanda);

        switch(com) {
            case acq_tast:
                printf("Inserire dati da aggiungere alla lista:\n");
                gets(dati);
                riempi_struct(&temp, dati);
                head = SortListIns(head, temp);
                break;
            case acq_file:
                printf("Inserire nome file:\n");
                scanf("%s", filename);
                fflush(stdin); //Ripulisco il buffer
                fin = fopen(filename, "r");
                while(!feof(fin)) {
                    fgets(dati, 50*4+6+11+5+6+1+1, fin); //50*4 max spazio delle stringhe, 6 caratteri per codice, 11 caratteri
                    //per data nascita, 5 caratteri CAP, 6 spazi e 1 \n, 1 valore aggiuntivo per il \0
                    riempi_struct(&temp, dati);
                    head = SortListIns(head, temp);
                }
                break;
            case ricerca:
                printf("Inserire codice da ricercare:\n");
                scanf("%s", cod);
                fflush(stdin);
                temp = SortListSearch(head, cod);
                if(strcmp(temp.codice, "B0000") != 0)
                    stamp(temp, stdout);
                else
                    printf("Nessun elemento con quel codice trovato!\n");
                break;
            case canc_codice:
                printf("Inserire codice:\n");
                scanf("%s", cod);
                fflush(stdin);
                temp = SortListDel_cod(&head, cod);
                if(strcmp(temp.codice, "B0000") != 0)
                    stamp(temp, stdout);
                else
                    printf("Nessun elemento da cancellare trovato!");
                break;
            case canc_date:
                printf("Inserire date:\n");
                scanf("%s %s", data1, data2);
                fflush(stdin);
                do {
                    temp = SortListDel_data(&head, data1, data2);
                    if(strcmp(temp.codice, "B0000") != 0) {
                        stamp(temp, stdout);
                        flag = 1;
                    }
                } while(strcmp(temp.codice, "B0000") != 0);
                if(!flag) {
                    printf("Nessun elemento da cancellare trovato!");
                }
                flag = 0;
                break;
            case stampa:

                if(head == NULL)
                    printf("Nessun elemento in lista!\n");
                else {
                    printf("Inserire nome file su cui stampare i dati:\n");
                    scanf("%s", filename);
                    fflush(stdin);
                    fout = fopen(filename, "w");
                    for (link x = head; x != NULL; x = x->next)
                        stamp(x->val, fout);
                }
                break;
            case fine:
                end = 0;
                break;
            default:
                printf("Comando non riconosciuto!\n");
                break;
        }
    }

    dealloca(head);
    if(fin != NULL)
        fclose(fin);
    if(fout != NULL)
        fclose(fout);
    return 0;
}

comando_e leggiComando(char domanda[]) {
    comando_e c;
    char cmd[LMAX];
    char table[err][LMAX] = {
            "acq_tast", "acq_file", "ric_cod", "canc_cod", "canc_date", "stampa", "fine"
    };

    sscanf(domanda, "%s", cmd);
    toLower(cmd);

    c = acq_tast;
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
link newNode(Item valore, link next) {
    link x = malloc(sizeof *x);
    if (x==NULL)
        return NULL;
    else {
        x->val = valore;
        x->next = next;
    }
    return x;
}

void riempi_struct(Item *temp, char *dati) {
    sscanf(dati, "%s %s %s %s %s %s %s\n", temp->codice,
           temp->nome, temp->cognome, temp->data_di_nascita, temp->via, temp->citta, temp->cap);
}

link SortListIns(link h, Item valore) {
    link x, p;
    int data1[3], data2[3];

    sscanf(valore.data_di_nascita, "%d/%d/%d", &(data2[2]), &(data2[1]), &(data2[0]));

    if (h==NULL || KeyGreater(KeyGet(h->val, data1), data2))
        return newNode(valore, h);
    for (x=h->next, p=h;
         x!=NULL && KeyGreater(data2,KeyGet(x->val, data1));
         p=x, x=x->next);
    p->next = newNode(valore, x);
    return h;
}

int *KeyGet(Item valore, int *data) {
    sscanf(valore.data_di_nascita, "%d/%d/%d", &(data[2]), &(data[1]), &(data[0]));
    return data;
}

int KeyGreater(int *data1, int *data2) {

    if(data2[0] > data1[0])
        return 1;
    if(data2[0] == data1[0] && data2[1] > data1[1])
        return 1;
    if(data2[0] == data1[0] && data2[1] == data1[1] && data2[2] > data1[2])
        return 1;
    return 0;
}

Item SortListSearch(link h, char *k) {
    link x;

    for (x=h; x!=NULL; x=x->next)
        if (KeyEq((x->val).codice, k))
            return x->val;
    return ITEMsetvoid();
}

int KeyEq(char *cod1, char *cod2) {

    if(strcmp(cod1, cod2) == 0)
        return 1;
    return 0;
}

Item ITEMsetvoid(void) {
    Item vuoto;

    strcpy(vuoto.codice, "B0000"); //Per formato di codice, impossibile da trovare

    return vuoto;
}

void stamp(Item persona, FILE *fout) {
    fprintf(fout, "%s %s %s %s %s %s %s\n", persona.codice, persona.nome, persona.cognome, persona.data_di_nascita, persona.via,
           persona.citta, persona.cap);
}

Item SortListDel_data(link *h, char *data1, char *data2) {
    Item temp = ITEMsetvoid();

    if (*h == NULL)
        return temp;
    for (link x = *h, p = NULL; x != NULL && KeyGeq(data1, (x->val).data_di_nascita);
         p=x, x=x->next) {
        if (KeyGeq((x->val).data_di_nascita, data2)){
            if (x == *h)
                *h = x->next;
            else
                p->next = x->next;
            temp = x->val;
            free(x);
            break;
        }
    }
    return temp;
}

int KeyGeq(char *data1, char *data2) {
    int date1[3], date2[3];
    sscanf(data1, "%d/%d/%d", &(date1[2]), &(date1[1]), &(date1[0]));
    sscanf(data2, "%d/%d/%d", &(date2[2]), &(date2[1]), &(date2[0]));

    if(date2[0] >= date1[0])
        return 1;
    if(date2[0] == date1[0] && date2[1] >= date1[1])
        return 1;
    if(date2[0] == date1[0] && date2[1] == date1[1] && date2[2] >= date1[2])
        return 1;
    return 0;
}

Item SortListDel_cod(link *h, char *k) {
    Item persona = ITEMsetvoid();
    if (*h == NULL)
        return persona;
    for (link x = *h, p = NULL; x != NULL; p=x, x=x->next) {
        if (KeyEq((x->val).codice,k)){
            if (x == *h) *h = x->next;
            else
                p->next = x->next;
            persona = x->val;
            free(x);
            break;
        }
    }
    return persona;
}

void dealloca(link head) {
    if(head == NULL) {
        return;
    }
    dealloca(head->next);
    free(head);
}