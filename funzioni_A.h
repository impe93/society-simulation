#ifndef __FUNZIONI_A__

#define __FUNZIONI_A__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>


/**
 * Inserisce i dati passati all'interno del parametro argv all'interno della struttura
 * di tipo caratteristiche_individuo puntata dal parametro p.
 * @param {caratteristiche_individuo*} p: riferimento a nuovo individuo in creazione
 * @param {char**} argv: dati passati a tipo_A.c dal gestore e contenti le caratteristiche 
 * del nuovo individuo (tipo, nome, genoma)
 */
void inserimento_caratteristiche_individuo(caratteristiche_individuo* p, char** argv);

/**
 * Inserisce i dati rappresentanti le caratteristiche del processo A all'interno di una 
 * rappresentazione_individuo libera all'interno della shm A.
 * @param {rappresentazione_individuo*} p_shm_A: puntatore che fa riferimento all'array di 
 * rappresentazione_individuo presente in shm A
 * @param {pid_t} pid: pid del processo A
 * @param {caratteristiche_individuo} individuo: individuo appena creato da registrare in shm A
 * @param {int} numero_A: indica numero di individui A presenti nella shm A
 */
void inserimento_in_shm_A(rappresentazione_individuo* p_shm_A, pid_t pid, caratteristiche_individuo individuo, int numero_A);

/**
 * Calcola il MCD tra il genoma dell'individuo A e il genoma dell'individuo B e ne ritorna il valore.
 * @param {unsigned long} genoma_A: genoma dell'individuo A
 * @param {unsigned long} genoma_B: genoma dell'individuo B
 * @return: ritorna l'unsigned long corrispondente al MCD tra genoma A e genoma B
 */
unsigned long mcd(unsigned long genoma_A, unsigned long genoma_B);

/**
 * Rimuove da shm_A i dati relativi al processo con pid corrispondente al parametro "pid_A".
 * @param {rappresentazione_individuo*} p_shm_A: puntatore a rappresentazione_individuo che punta
 * a un segmento di memeria condivisa
 * @param {pid_t} pid_A: pid dell'individuo A da rimuovere
 * @param {int} individui_A: numero di individui A presenti all'interno della shm_A.
 */
void rimozione_da_shm_A(rappresentazione_individuo* p_shm_A, pid_t pid_A, int individui_A);


void inserimento_caratteristiche_individuo(caratteristiche_individuo* p, char** argv){
    p->tipo = *argv[0];
    strcpy(p->nome, *(argv + 1));
    p->genoma = atol(*(argv + 2));
}

void inserimento_in_shm_A(rappresentazione_individuo* p_shm_A, pid_t pid, caratteristiche_individuo individuo, int numero_A){
    bool inserito_in_shm_A = FALSE;
    for(int i = 0; inserito_in_shm_A == FALSE && i < numero_A; i++){
        if(p_shm_A[i].utilizzata == FALSE){
            p_shm_A[i].pid = pid;
            p_shm_A[i].caratteristiche = individuo;
            p_shm_A[i].utilizzata = TRUE;
            inserito_in_shm_A = TRUE;
        }
    }
}

unsigned long mcd(unsigned long genoma_A, unsigned long genoma_B){
    if (genoma_B == genoma_A)
        return genoma_B;
    if (genoma_A < genoma_B)  
        return mcd(genoma_A - genoma_B, genoma_B);
    return mcd(genoma_A, genoma_B - genoma_A);
}

void rimozione_da_shm_A(rappresentazione_individuo* p_shm_A, pid_t pid_A, int individui_A){
    bool rimosso = FALSE;
    for(int i = 0; rimosso == FALSE && i < individui_A; i++){
        if(p_shm_A[i].pid == pid_A){
            p_shm_A[i].utilizzata = FALSE;
            rimosso = TRUE;
        }
    }
}

#endif