#ifndef __GESTIONE_SEMAFORI__

#define __GESTIONE_SEMAFORI__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <ctype.h>

#include "sem_protocol.h"

/**
 * Crea un semaforo con chiave uguale al parametro "chiave" e ritorna l'ID del
 * semaforo appena creato.
 * @param {int} chiave: rappresenta la chiave per la creazione del semaforo
 * @return {int}: ritorna l'ID del semaforo appena creato, oppure in caso di errore 
 * durante la creazione il processo termina.
 */
int sem_creazione(int chiave);

/**
 * Recupera un semaforo con chiave uguale al parametro "chiave" e ritorna l'ID del
 * semaforo appena creato.
 * @param {int} chiave: rappresenta la chiave per il recupero del semaforo
 * @return {int}: ritorna un intero che rappresenta l'ID del semaforo appena recuperato, oppure in
 * caso di errore durante il recupero il processo termina.
 */
int sem_recupero(int chiave);

/**
 * Inizializza il semaforo numero 0 del set con ID uguale al parametro "id" come
 * disponibile.
 * @param {int} id: rappresenta l'ID del set che si vuole inizializzare
 * @error: In caso di errore il processo conclude
 */
void sem_init_disponibile(int id);

/**
 * Inizializza il semaforo numero 0 del set con ID uguale al parametro "id" come
 * occupato.
 * @param {int} id: rappresenta l'ID del set che si vuole inizializzare
 * @error: In caso di errore il processo conclude
 */
void sem_init_occupato(int id);

/**
 * Riserva il semaforo numero 0 del set con ID uguale al parametro "id".
 * @param {int} id: Un intero che rappresenta l'ID del set a cui appartiene il semaforo 
 * che si vuole riservare
 * @error: In caso di errore il processo conclude
 */
void sem_riserva(int id);

/**
 * Rilascia il semaforo numero 0 del set con ID uguale al parametro "id".
 * @param {int} id: Un intero che rappresenta l'ID del set a cui appartiene il semaforo che 
 * si vuole rilasciare
 * @error: In caso di errore il processo conclude
 */
void sem_rilascia(int id);

/**
 * Cancella il set di semafori con ID uguale al parametro "id".
 * @param {int} id: rappresenta l'ID del set di semafori che si vuole cancellare
 * @error: In caso di errore il processo conclude
 */
void sem_cancella(int id);

#endif
