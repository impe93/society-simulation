#ifndef __FUNZIONI_B__

#define __FUNZIONI_B__

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
 * Inserisce i dati rappresentanti le caratteristiche del processo B all'interno di una 
 * rappresentazione_individuo libera all'interno della shm B.
 * @param {rappresentazione_individuo*} p_shm_B: puntatore che fa riferimento all'array di 
 * rappresentazione_individuo presente in shm B
 * @param {pid_t} pid: pid del processo B
 * @param {caratteristiche_individuo} individuo: individuo appena creato da registrare in shm B
 * @param {int} numero_B: indica numero di individui B presenti nella shm B
 */
void inserimento_in_shm_B(rappresentazione_individuo* p_shm_B, pid_t pid, caratteristiche_individuo individuo, int numero_B);

/**
 * Calcola il MCD tra il genoma dell'individuo A e il genoma dell'individuo B e ne ritorna il valore.
 * @param {unsigned long} genoma_A: genoma dell'individuo A
 * @param {unsigned long} genoma_B: genoma dell'individuo B
 * @return: ritorna l'unsigned long corrispondente al MCD tra genoma A e genoma B
 */
unsigned long mcd(unsigned long genoma_A, unsigned long genoma_B);

/**
 * Seleziona l'individuo A all'interno della shm_A che massimizza il pid di un eventuale figlio e 
 * lo inserisce in individuo_per_accoppiamento puntato dal parametro "individuo_A". 
 * @param {rappresentazione_individuo*} p_shm_A: puntatore all'array in shm dove sono memorizzati gli
 * individui A
 * @param {individuo_per_accoppiamento*} individuo_A: puntatore al individuo_per_accoppiamento dove viene
 * inserito l'individuo A scelto per l'accoppiamento
 * @param {unsigned long} genoma_B: genoma dell'individuo B
 * @param {int} numero_A: numero di individui A rappresentabili nell'array della shared memory
 */
void seleziona_individuo_A_ideale(rappresentazione_individuo* p_shm_A, individuo_per_accoppiamento* individuo_A, 
                    unsigned long genoma_B, int numero_A);

/**
 * Inserisce i dati contenuti nei parametri "pid_B" e "individuo_B" all'interno di 
 * individuo_per_accoppiamento puntato dal parametro "messaggio_accoppiamento".
 * @param {individuo_per_accoppiamento*} messaggio_accoppiamento: puntatore a individuo_per_accoppiamento 
 * dove verranno inseriti i dati
 * @param {pid_t} pid_B: pid dell'individuo B
 * @param {caratteristiche_individuo} individuo_B: individuo_B che desidera mandare richiesta di 
 * accoppiamento ad un processo A.
 */
void inserimento_in_messaggio_accoppiamento(individuo_per_accoppiamento* messaggio_accoppiamento, 
                    pid_t pid_B, caratteristiche_individuo individuo_B);

/**
 * Rimuove da shm_B i dati relativi al processo con pid corrispondente al parametro "pid_B".
 * @param {rappresentazione_individuo*} p_shm_B: puntatore a rappresentazione_individuo che punta
 * a un segmento di memeria condivisa
 * @param {pid_t} pid_B: pid dell'individuo B da rimuovere
 * @param {int} individui_B: numero di individui B presenti all'interno della shm_B.
 */
void rimozione_da_shm_B(rappresentazione_individuo* p_shm_B, pid_t pid_B, int individui_B);


void inserimento_caratteristiche_individuo(caratteristiche_individuo* p, char** argv){
    printf("Genoma B = %s\n", *(argv + 2));
    (*p).tipo = *argv[0];
    strcpy(p->nome, *(argv + 1));
    (*p).genoma = atol(*(argv + 2));
}

void inserimento_in_shm_B(rappresentazione_individuo* p_shm_B, pid_t pid, caratteristiche_individuo individuo, int numero_B){
    bool inserito_in_shm_B = FALSE;
    for(int i = 0; inserito_in_shm_B == FALSE && i < numero_B; i++){
        if(p_shm_B[i].utilizzata == FALSE){
            p_shm_B[i].pid = pid;
            p_shm_B[i].caratteristiche = individuo;
            p_shm_B[i].utilizzata = TRUE;
            inserito_in_shm_B = TRUE;
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

void seleziona_individuo_A_ideale(rappresentazione_individuo* p_shm_A, individuo_per_accoppiamento* individuo_A, 
                    unsigned long genoma_B, int numero_A){
    
    int indice_A_ideale = 0;
    int genoma_massimo = 0;
    for(int i = 0; i < numero_A; i++){
        if(p_shm_A[i].utilizzata == TRUE){
            int mcd_genomi = mcd(p_shm_A[i].caratteristiche.genoma, genoma_B);
            if(mcd_genomi >= genoma_massimo){
                genoma_massimo = mcd_genomi;
                indice_A_ideale = i;
            }
        }
    }
    (*individuo_A).caratteristiche = p_shm_A[indice_A_ideale].caratteristiche;
    (*individuo_A).pid = p_shm_A[indice_A_ideale].pid;
}

void inserimento_in_messaggio_accoppiamento(individuo_per_accoppiamento* messaggio_accoppiamento, 
                    pid_t pid_B, caratteristiche_individuo individuo_B){
    (*messaggio_accoppiamento).pid = pid_B;
    (*messaggio_accoppiamento).caratteristiche = individuo_B;
}

void rimozione_da_shm_B(rappresentazione_individuo* p_shm_B, pid_t pid_B, int individui_B){
    bool rimosso = FALSE;
    for(int i = 0; rimosso == FALSE && i < individui_B; i++){
        if(p_shm_B[i].pid == pid_B){
            p_shm_B[i].utilizzata = FALSE;
            rimosso = TRUE;
        }
    }
}

#endif