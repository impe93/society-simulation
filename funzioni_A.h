#ifndef __FUNZIONI_A__

#define __FUNZIONI_A__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Handler da associare al segnale SIGTERM che cambia il valore della variabile globale
 * pronto_a_terminare in 1, in modo da notificare al processo A che deve terminare.
 */
void signal_handler(int sig);

/**
 * Associa un handler (signal_handler()) al segnale SIGTERM.
 */
void associazione_handler();

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
 * @param {rappresentazione_individuo**} p_shm_A: puntatore di puntatori che fa riferimento
 * alla lista di rappresentazione_individuo presente in shm A
 * @param {pid_t} pid: pid del processo A
 * @param {caratteristiche_individuo} individuo: individuo appena creato da registrare in shm A
 * @param {int} numero_A: numero massimo di individui A presenti nella shm A
 */
void inserimento_in_shm_A(rappresentazione_individuo** p_shm_A, pid_t pid, caratteristiche_individuo individuo);

/**
 * Calcola il MCD tra il genoma dell'individuo A e il genoma dell'individuo B e ne ritorna il valore.
 * @param {unsigned long} genoma_A: genoma dell'individuo A
 * @param {unsigned long} genoma_B: genoma dell'individuo B
 * @return: ritorna l'unsigned long corrispondente al MCD tra genoma A e genoma B
 */
unsigned long mcd(unsigned long genoma_A, unsigned long genoma_B);


void signal_handler(int sig){
    pronto_a_terminare = 1;
}

void associazione_handler(){
    if(signal(SIGTERM, signal_handler) == SIG_ERR){
        printf("errore per segnale SIGTERM\n");
        exit(EXIT_FAILURE);
    }
}

void inserimento_caratteristiche_individuo(caratteristiche_individuo* p, char** argv){
    p->tipo = *argv[0];
    strcpy(p->nome, *(argv + 1));
    p->genoma = atol(*(argv + 2));
}

void inserimento_in_shm_A(rappresentazione_individuo** p_shm_A, pid_t pid, caratteristiche_individuo individuo, int numero_A){
    bool inserito_in_shm_A = FALSE;
    for(int i = 0; inserito_in_shm_A == FALSE && i < numero_A; i++){
        if((*(p_shm_A + i))->utilizzata == FALSE){
            (*(*(p_shm_A + i))).pid = pid;
            (*(*(p_shm_A + i))).caratteristiche = individuo;
            (*(*(p_shm_A + i))).utilizzata = TRUE;
            inserito_in_shm_A = TRUE;
            printf("trovata struct dove inserire info sul processo A in shmA\n");
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

#endif