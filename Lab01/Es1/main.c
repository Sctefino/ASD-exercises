#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *cercaRegexp(char *src, char *regexp);
void reset(char **p_src, char **p_regexp, int *count1, int *count2, int *count_eff);
void increase(char **p_src, char **p_regexp, int *count1, int *count2, int *count_eff);

int main(void) {
    char regexp[20] = "\\a.[eszt]", src[100] = "che bella moto", *p;

    p = cercaRegexp(src, regexp);
    printf("%s", p);

    return 0;
}

char *cercaRegexp(char *src, char *regexp) {
    char *p_src = src, *p_regexp = regexp;
    int count1 = 0, count2 = 0, count_eff = 0, flag = 1, x; //count_eff serve a contare la lunghezza effettiva della regexp,
                                                            // considerando unico il metacarattere

    while(count1 < strlen(src)) {
        //Primo giro: trova la lettera corrispondente
        if(isalpha(*p_regexp) != 0 && strcmp(regexp, p_regexp) == 0){
            x = strlen(p_src);
            for(int i = 0; i < x && *p_src != *p_regexp; i++) {
                p_src++;
                count1++;
            }
            increase(&p_src, &p_regexp,&count1, &count2, &count_eff);
        }
            //Giro normale: controlla che la lettera dove ti trovi corrisponda
        else if(isalpha(*p_regexp) != 0) {
            if(*p_regexp != *p_src) {
                reset(&p_src, &p_regexp, &count1, &count2, &count_eff);
            }
            else {
                increase(&p_src, &p_regexp, &count1, &count2, &count_eff);
            }
            if(count2 == strlen(regexp))
                return p_src-count_eff;
        }
            //Carattere speciale
        else {
            //Suddivisone in casi
            switch(*p_regexp) {
                case '.':
                    if(isalpha(*p_src) != 0) {
                        increase(&p_src, &p_regexp, &count1, &count2, &count_eff);
                    }
                    else {
                        reset(&p_src, &p_regexp, &count1, &count2, &count_eff);
                    }
                    if(count2 == strlen(regexp)) {
                        return p_src-count_eff;
                    }
                    break;
                case '\\':
                    p_regexp++;
                    count2++;
                    if(*p_regexp == 'a') {
                        if(*p_src >= 'a' && *p_src <= 'z') {
                            increase(&p_src, &p_regexp, &count1, &count2, &count_eff);
                        }
                        else {
                            reset(&p_src, &p_regexp, &count1, &count2, &count_eff);
                        }
                    }
                    else if(*p_regexp == 'A') {
                        if(*p_src >= 'A' && *p_src <= 'Z') {
                            increase(&p_src, &p_regexp, &count1, &count2, &count_eff);
                        }
                        else {
                            reset(&p_src, &p_regexp, &count1, &count2, &count_eff);
                        }
                    }
                    else
                        return NULL;

                    if(count2 == strlen(regexp)) {
                        return p_src-count_eff;
                    }
                    break;
                case '[':
                    p_regexp++;
                    count2++;
                    if(*p_regexp == '^') {
                        do {
                            p_regexp++;
                            count2++;
                            if(*p_regexp == *p_src)
                                flag = 0;
                        } while(*p_regexp != ']');
                        if(flag) {
                            increase(&p_src, &p_regexp, &count1, &count2, &count_eff);
                        }
                        else {
                            reset(&p_src, &p_regexp, &count1, &count2, &count_eff);
                        }
                        flag = 1;
                        if(count2 == strlen(regexp))
                            return p_src-count_eff;
                    }
                    else {
                        do {
                            if(*p_regexp == *p_src)
                                flag = 0;
                            p_regexp++;
                            count2++;
                        } while(*p_regexp != ']');
                        if(!flag) {
                            increase(&p_src, &p_regexp, &count1, &count2, &count_eff);
                        }
                        else {
                            reset(&p_src, &p_regexp, &count1, &count2, &count_eff);
                        }
                        flag = 1;
                        if(count2 == strlen(regexp))
                            return p_src-count_eff;
                    }
                    break;
                default:
                    return NULL;
            }
        }
    }
    return NULL;
}

void reset(char **p_src, char **p_regexp, int *count1, int *count2, int *count_eff) {
    *p_regexp -= *count2;
    *p_src -= (*count_eff-1);
    *count1 -= (*count_eff-1);
    *count2 = 0;
    *count_eff = 0;
}

void increase(char **p_src, char **p_regexp, int *count1, int *count2, int *count_eff) {
    *p_regexp += 1;
    *p_src += 1;
    *count1 += 1;
    *count2 += 1;
    *count_eff += 1;
}