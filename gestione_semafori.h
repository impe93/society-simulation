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
 * Crea un semaforo con chiave uguale al paramtero "chiave" e ritorna l'ID del
 * semaforo appena creato
 * @param chiave: Un intero che rappresenta la chiave per la creazione del semaforo
 * @return: ritorna un intero che rappresenta l'ID del semaforo appena creato, oppure in
 * caso di errore durante la creazione il processo termina.
 */
int sem_creazione(int chiave) {
    int id = 0;
    if((id = semget(chiave, 1, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        printf("Erorre durante la creazione del semaforo con ID = %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * Recupera un semaforo con chiave uguale al parametro "chiave" e ritorna l'ID del
 * semaforo appena creato
 * @param chiave: Un intero che rappresenta la chiave per il recupero del semaforo
 * @return: ritorna un intero che rappresenta l'ID del semaforo appena recuperato, oppure in
 * caso di errore durante il recupero il processo termina.
 */
int sem_recupero(int chiave) {
    int id = 0;
    if((id = semget(chiave, 1, 0)) == -1) {
        printf("Erorre durante il recupero del semaforo con ID = %i.\n", chiave);
        DEBUG;
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * Inizializza il semaforo numero 0 del set con ID uguale al parametro "id" come
 * disponibile
 * @param id: Un intero che rappresenta l'ID del set che si vuole inizializzare
 * @error: In caso di errore il processo conclude
 */
void sem_init_disponibile(int id) {
    if (initSemAvailable(id, 0) == -1) {
        printf("Errore durante l'inizializzazione a disponibile del semaforo con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

/**
 * Inizializza il semaforo numero 0 del set con ID uguale al parametro "id" come
 * occupato
 * @param id: Un intero che rappresenta l'ID del set che si vuole inizializzare
 * @error: In caso di errore il processo conclude
 */
void sem_init_occupato(int id) {
    if (initSemInUse(id, 0) == -1) {
        printf("Errore durante l'inizializzazione a occupato del semaforo con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

/**
 * Riserva il semaforo numero 0 del set con ID uguale al parametro "id"
 * @param id: Un intero che rappresenta l'ID del set a cui appartiene il semaforo che si vuole
 * riservare
 * @error: In caso di errore il processo conclude
 */
void sem_riserva(int id) {
    if (reserveSem(id, 0) == -1) {
        printf("Errore durante la riserva del semaforo con ID = %i e PID = %i.\n", id, getpid());
        exit(EXIT_FAILURE);
    }
}

/**
 * Rilascia il semaforo numero 0 del set con ID uguale al parametro "id"
 * @param id: Un intero che rappresenta l'ID del set a cui appartiene il semaforo che si vuole
 * rilasciare
 * @error: In caso di errore il processo conclude
 */
void sem_rilascia(int id) {
    if (releaseSem(id, 0) == -1) {
        printf("Errore durante il rilascio del semaforo con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

/**
 * Cancella il set di semafori con ID uguale al parametro "id"
 * @param id: Un intero che rappresenta l'ID del set di semafori che si vuole cancellare
 * @error: In caso di errore il processo conclude
 */
void sem_cancella(int id) {
    if (semctl(id, 0, IPC_RMID) == -1) {
        printf("Errore durante la rimozione del semaforo con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

#endif
