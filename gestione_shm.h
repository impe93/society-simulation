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

int shm_creazione(int chiave, int grandezza_array) {
    int id = 0;
    if ((id = shmget(id, sizeof(rappresentazione_individuo) * grandezza_array, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        printf("Errore durante la creazione della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

int shm_recupero(int chiave, int grandezza_array) {
    int id = 0;
    if ((id = shmget(id, sizeof(rappresentazione_individuo) * grandezza_array, 0)) == -1) {
        printf("Errore durante la creazione della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

#endif