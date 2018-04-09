#ifndef __FUNZIONI_A__

#define __FUNZIONI_A__

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
    p->tipo = *argv[0];
    strcpy(p->nome, *(argv + 1));
    p->genoma = atol(*(argv + 2));
}

/**
 * Inserisce i dati rappresentanti le caratteristiche del processo A all'interno di una 
 * rappresentazione_individuo libera all'interno della shm A.
 * @param {rappresentazione_individuo**} p_shm_A: puntatore di puntatori che fa riferimento
 * alla lista di rappresentazione_individuo presente in shm A
 * @param {pid_t} pid: pid del processo A
 * @param {caratteristiche_individuo} individuo: individuo appena creato da registrare in shm A
 */
void inserimento_in_shm_A(rappresentazione_individuo** p_shm_A, pid_t pid, caratteristiche_individuo individuo){
    bool inserito_in_shm_A = FALSE;
    for(int i = 0; inserito_in_shm_A == FALSE; i++){
        if((*(p_shm_A + i))->utilizzata == FALSE){
            (*(*(p_shm_A + i))).pid = pid;
            (*(*(p_shm_A + i))).caratteristiche = individuo;
            (*(*(p_shm_A + i))).utilizzata = TRUE;
            inserito_in_shm_A = TRUE;
            printf("trovata struct dove inserire info sul processo A in shmA\n");
        }
    }
}

#endif