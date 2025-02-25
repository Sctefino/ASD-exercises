#include "pg.h"
void annulla_stat(stat_t *stat) {
    stat->hp = 0;
    stat->mp = 0;
    stat->atk = 0;
    stat->def = 0;
    stat->mag = 0;
    stat->spr = 0;
}

stat_t sum_stat(stat_t s1, stat_t s2) {
    stat_t sum;
    sum.hp = s1.hp + s2.hp;
    sum.mp = s1.mp + s2.mp;
    sum.atk = s1.atk + s2.atk;
    sum.def = s1.def + s2.def;
    sum.mag = s1.mag + s2.mag;
    sum.spr = s1.spr + s2.spr;

    return sum;
}

int pg_read(FILE *fp, pg_t *pgp) {
    int x = 0;

    if (!feof(fp)) {
        x = fscanf(fp, "%s %s %s %d %d %d %d %d %d", pgp->cod, pgp->nome, pgp->classe, &(pgp->b_stat.hp),
                   &(pgp->b_stat.mp), &(pgp->b_stat.atk), &(pgp->b_stat.def), &(pgp->b_stat.mag), &(pgp->b_stat.spr));
        if(fp != stdin)
            fscanf(fp, "\n");
        else
            fflush(stdin);

        annulla_stat(&(pgp->eq_stat));
        pgp->equip = equipArray_init();
    }
    return x;
}

void pg_clean(pg_t *pgp) {
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray) {
    stat_t sum = sum_stat(pgp->b_stat, pgp->eq_stat);

    fprintf(fp, "%s %s %s ", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &(sum), MIN_STAT);
    equipArray_print(fp, pgp->equip, invArray);

}

void pg_updateEquip(pg_t *pgp, invArray_t invArray) {
    int i = 0, x;

    equipArray_update(pgp->equip, invArray);
    annulla_stat(&(pgp->eq_stat));

    while(i < equipArray_inUse(pgp->equip)) {
        x = equipArray_getEquipByIndex(pgp->equip, i);
        if(x != -1) {
            pgp->eq_stat = sum_stat(pgp->eq_stat, inv_getStat(invArray_getByIndex(invArray, x)));
            i++;
        }
    }

}