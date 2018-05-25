#ifndef __GESTIONE_SHM__

#define __GESTIONE_SHM__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#include "tipi_simulatore_societa.h"

/**
 * Crea un nuovo segmento di shared memory grande ("grandezza_array"*sizeof(rappresentazione_individuo))
 * utilizzando come chiave il parametro "chiave" e restituisce l'ID  del segmento appena creato.
 * @param {int} chiave: chiave utilizzata per la creazione del nuovo segmento di shm
 * @param {int} grandezza_array: numero di elementi di tipo rappresentazione_individuo per i quali si 
 * vuole allocare spazio nella shm
 * @return {int}: ritorna l'ID del segmento di shm appena creato, in caso di errore durante la creazione
 * chiude il programma
 */
int shm_creazione(int chiave, int grandezza_array);

/**
 * Crea un nuovo segmento di shared memory grande quanto la struttura "descrizione_simulazione"
 * utilizzando come chiave il parametro "chiave" passato come parametro e restituisce l'ID del
 * segmento appena creato.
 * @param {int} chiave: La chiave utilizzata dalla shared memory
 */
int shm_creazione_descrizione(int chiave);

/**
 * Restituisce l'ID di un segmento di shm esistente utilizzando la sua chiave (parametro "chiave") e 
 * la sua grandezza (parametro "grandezza_array").
 * @param {int} chiave: chiave utilizzata per il recupero dell'ID del segmento di shm
 * @param {int} grandezza_array: numero di elementi di tipo rappresentazione_individuo per i quali si 
 * è allocato spazio nella shm
 * @return {int}: ritorna l'ID del segmento di shm esistente, in caso di errore durante la creazione
 * chiude il programma
 */
int shm_recupero(int chiave, int grandezza_array);

/**
 * Restituisce l'ID di un segmento di shm esistente di grandezza uguale alla struttura 
 * "descrizione_simulazione" utilizzando la sua chiave (parametro "chiave").
 * @param {int} chiave: chiave utilizzata per il recupero dell'ID del segmento di shm
 * @return {int}: ritorna l'ID del segmento di shm già esistente, in caso di errore durante 
 * il recupero chiude il programma
 */
int shm_recupero_descrizione(int chiave);

/**
 * Attacca il segmento di shm con ID corrispondente al parametro "shmid" alla memoria virtuale
 * del processo chiamante e inserisce nel parametro "p" un puntatore all'inizio del segmento di SM
 * nello spazio di indirizzi virtuale di tale processo.
 * @param {int} shmid: intero rappresentante l'ID del segmento di shm da attaccare alla memoria virtuale 
 * del processo
 * @param {rappresentazione_individuo**} p: void* in cui il metodo inserisce un puntatore 
 * all'inizio del segmento di SM nello spazio di indirizzi virtuale del processo chiamante
 */
void shm_attach_rappresentazione_individuo(int shmid, rappresentazione_individuo** p);

/**
 * Attacca il segmento di shm con ID corrispondente al parametro "shmid" alla memoria virtuale
 * del processo chiamante e inserisce nel parametro "p" un puntatore all'inizio del segmento di SM
 * nello spazio di indirizzi virtuale di tale processo.
 * @param {int} shmid: intero rappresentante l'ID del segmento di shm da attaccare alla memoria virtuale 
 * del processo
 * @param {descrizione_simulazione**} p: void* in cui il metodo inserisce un puntatore all'inizio 
 * segmento di SM nello spazio di indirizzi virtuale del processo chiamante
 */
void shm_attach_descrizione_simulazione(int shmid, descrizione_simulazione** p);

/**
 * Stacca l'area di shm dallo spazio degli indirizzi del processo chiamante utilizzando 
 * l’indirizzo del punto di accesso a tale area (restituito dalla shmat()) e contenuto 
 * nel parametro "shmaddr".
 * @param {rappresentazione_individuo*} shmaddr: indirizzo del punto di accesso all'area di shm 
 * dallo spazio degli indirizzi del processo chiamante
 */
void shm_detach_rappresentazione_individuo(rappresentazione_individuo* shmaddr);

/**
 * Stacca l'area di shm dallo spazio degli indirizzi del processo chiamante utilizzando 
 * l’indirizzo del punto di accesso a tale area (restituito dalla shmat()) e contenuto 
 * nel parametro "shmaddr".
 * @param {rappresentazione_individuo*} shmaddr: indirizzo del punto di accesso all'area di shm 
 * dallo spazio degli indirizzi del processo chiamante
 */
void shm_detach_descrizione_simulazione(descrizione_simulazione* shmaddr);

/**
 * Cancella il segmento di shm con ID corrispondente al parametro "shmid" 
 * (ma solo dopo che tutti i processi che si erano correttamente attacati ad esso si sono staccati).
 * @param shmid: ID del segmento di shm da cancellare
 */
void shm_remove(int shmid);

#endif
