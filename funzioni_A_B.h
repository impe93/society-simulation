#ifndef __FUNZIONI_A_B__

#define __FUNZIONI_A_B__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

#include "tipi_simulatore_societa.h"

/**
 * Inserisce i dati passati all'interno del parametro argv all'interno della struttura
 * caratteristiche_individuo puntata dal parametro p.
 * @param {caratteristiche_individuo*} p: riferimento al nuovo individuo in creazione
 * @param {char**} argv: dati passati al processo A (B) dal gestore e contenenti le 
 * caratteristiche del nuovo individuo (tipo, nome, genoma)
 */
void inserimento_caratteristiche_individuo(caratteristiche_individuo* p, char** argv);

/**
 * Inserisce i dati rappresentanti le caratteristiche del processo A (B) all'interno di una 
 * rappresentazione_individuo libera all'interno della shm A (shm B).
 * @param {rappresentazione_individuo*} p_shm: puntatore che fa riferimento all'array di 
 * rappresentazione_individuo presente in shm A (shm B)
 * @param {pid_t} pid: pid del processo in creazione
 * @param {caratteristiche_individuo} individuo: individuo appena creato da registrare in shm
 * @param {int} numero_individui: numero di individui A (B) che possono essere contenuti 
 * in shm A (shm B)
 */
void inserimento_in_shm(rappresentazione_individuo* p_shm, pid_t pid, caratteristiche_individuo individuo, int numero_individui);

/**
 * Calcola il MCD tra il genoma dell'individuo A e il genoma dell'individuo B e ne ritorna il valore.
 * @param {unsigned long} a: genoma dell'individuo A
 * @param {unsigned long} b: genoma dell'individuo B
 * @return {unsigned long}: l'unsigned long corrispondente al MCD tra genoma di A e genoma di B
 */
unsigned long mcd(unsigned long a, unsigned long b);

/**
 * Rimuove da shm A (shm B) i dati relativi al processo con PID corrispondente al parametro "pid".
 * @param {rappresentazione_individuo*} p_shm: puntatore che fa riferimento all'array di 
 * rappresentazione_individuo presente in shm A (shm B)
 * @param {pid_t} pid: pid dell'individuo A (B) da rimuovere
 * @param {int} numero_individui: numero di individui A (B) che possono essere contenuti 
 * nella shm A (shm B)
 */
void rimozione_da_shm(rappresentazione_individuo* p_shm, pid_t pid, int numero_individui);

#endif
