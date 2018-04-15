#ifndef __GESTIONE_SHM__

#define __GESTIONE_SHM__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <ctype.h>

#include "tipi_simulatore_societa.h"

/**
 * Crea un nuovo segmento di shared memory grande ("grandezza_array"*sizeof(rappresentazione_individuo)) bytes
 * utilizzando come chiave il parametro "chiave" e restituisce l'ID  del segmento appena creato.
 * 
 * @param chiave: chiave utilizzata per la creazione del nuovo segmento di shm
 * @param grandezza_array: numero di elementi di tipo rappresentazione_individuo per i quali si 
 * vuole allocare spazio nella shm
 * @return: ritorna l'ID del segmento di shm appena creato, in caso di errore durante la creazione
 * chiude il programma
 */
int shm_creazione(int chiave, int grandezza_array) {
    int id = 0;
    if ((id = shmget(id, sizeof(rappresentazione_individuo) * grandezza_array, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        printf("Errore durante la creazione della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * Crea un nuovo segmento di shared memory grande quanto la struttura "descrizione_simulazione"
 * utilizzando come chiave il parametro "chiave" passato come parametro e restituisce l'ID del
 * segmento appena creato
 * 
 * @param {int} chiave: La chiave utilizzata dalla shared memory
 */
int shm_creazione_descrizione(int chiave) {
    int id = 0;
    if ((id = shmget(id, sizeof(descrizione_simulazione), IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        printf("Errore durante la creazione della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * Restituisce l'ID di un segmento di shm esistente utilizzando la sua chiave (parametro "chiave") e 
 * la sua grandezza (parametro "grandezza_array").
 * 
 * @param chiave: chiave utilizzata per il recupero dell'ID del segmento di shm
 * @param grandezza_array: numero di elementi di tipo rappresentazione_individuo per i quali si 
 * è allocato spazio nella shm
 * @return: ritorna l'ID del segmento di shm appena esistente, in caso di errore durante la creazione
 * chiude il programma
 */
int shm_recupero(int chiave, int grandezza_array) {
    int id = 0;
    if ((id = shmget(chiave, sizeof(rappresentazione_individuo) * grandezza_array, 0)) == -1) {
        printf("Errore durante la creazione della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * Restituisce l'ID di un segmento di shm esistente utilizzando la sua chiave (parametro "chiave") di
 * grandezza uguale alla struttura "descrizione_simulazione"
 * 
 * @param {int} chiave: Chiave utilizzata per il recupero dell'ID del segmento di shm
 * @return: ritorna l'ID del segmento di shm già esistente
 * @error: Terminazione del processo che ha eseguito il metodo
 */
int shm_recupero_descrizione(int chiave) {
    int id = 0;
    if ((id = shmget(chiave, sizeof(descrizione_simulazione), 0)) == -1) {
        printf("Errore durante la creazione della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * Attacca il segmento di shm con ID corrispondente al parametro "shmid" alla memoria virtuale
 * del processo chiamante e inserisce nel parametro "p" un puntatore all'inizio del segmento di SM
 * nello spazio di indirizzi virtuale di tale processo.
 * 
 * @param shmid: intero rappresentante l'ID del segmento di shm da attaccare alla memoria virtuale 
 * del processo
 * @param p: void* in cui il metodo inserisce un puntatore all'inizio del segmento di SM nello 
 * spazio di indirizzi virtuale del processo chiamante
 */
void shm_attach(int shmid, void* p) {
    if((p = shmat(shmid, NULL, 0)) == (void*)-1) {
        printf("errore in attacco del segmento di shm con id %i\n", shmid);
        exit(EXIT_FAILURE);
    }
}

/**
 * Stacca l'area di shm dallo spazio degli indirizzi del processo chiamante utilizzando 
 * l’indirizzo del punto di accesso a tale area (restituito dalla shmat()) e contenuto 
 * nel parametro "shmaddr".
 * 
 * @param shmaddr: indirizzo del punto di accesso all'area di shm dallo spazio degli
 * indirizzi del processo chiamante
 */
void shm_detach(void* shmaddr){
    if(shmdt(shmaddr) == -1){
        printf("errore nello staccamento del segmento di shm\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Cancella il segmento di shm con ID corrispondente al parametro "shmid" 
 * (ma solo dopo che tutti i processi che si erano correttamente attacati ad esso si sono staccati).
 * 
 * @param shmid: ID del segmento di shm da cancellare
 */
void shm_remove(int shmid){
    if(shmctl(shmid, IPC_RMID, 0) == -1){
        printf("errore nella rimozione del frammento di shm\n");
        exit(EXIT_FAILURE);
    }
}

#endif