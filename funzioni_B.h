#ifndef __FUNZIONI_B__

#define __FUNZIONI_B__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

#include "tipi_simulatore_societa.h"
#include "gestione_code.h"
#include "funzioni_A_B.h"


/**
 * Seleziona l'individuo A all'interno della shm_A che massimizza il pid di un eventuale figlio e 
 * lo inserisce in individuo_per_accoppiamento puntato dal parametro "individuo_A". 
 * @param {rappresentazione_individuo*} p_shm_A: puntatore all'array in shm dove sono memorizzati gli
 * individui A
 * @param {individuo_per_accoppiamento*} individuo_A: puntatore al individuo_per_accoppiamento dove viene
 * inserito l'individuo A scelto per l'accoppiamento
 * @param {unsigned long} genoma_B: genoma dell'individuo B
 * @param {int} numero_A: numero di individui A che possono essere contenuti nella shm A
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

#endif
