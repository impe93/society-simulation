#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Inserisce i dati passati all'interno del parametro argv all'interno della struttura
 * di tipo caratteristiche_individuo puntata dal parametro p.
 * @param {caratteristiche_individuo*} p: riferimento a nuovo individuo in creazione
 * @param {char**} argv: dati passati a tipo_A.c dal gestore e contenti le caratteristiche 
 * del nuovo individuo (tipo, nome, genoma)
 */
void inserimento_caratteristiche_individuo(caratteristiche_individuo* p, char** argv){
    p->tipo = *(argv + 1)[0];
    strcpy(p->nome, *(argv + 2));
    p->genoma = atol(*(argv + 3));
}

