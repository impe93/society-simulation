#include <stdio.h>
#include <unistd.h>

#include "tipi_simulatore_societa.h"
#include "gestione_semafori.h"
#include "gestione_code.h"
#include "gestione_shm.h"

int main() {
    msg_rimuovi_coda(msg_recupera_coda(1240));
    msg_rimuovi_coda(msg_recupera_coda(1241));
    msg_rimuovi_coda(msg_recupera_coda(1242));
    sem_cancella(sem_recupero(1236));
    sem_cancella(sem_recupero(1237));
    sem_cancella(sem_recupero(1238));
    sem_cancella(sem_recupero(1239));
    sem_cancella(sem_recupero(1244));
    sem_cancella(sem_recupero(1245));
    sem_cancella(sem_recupero(1246));
    sem_cancella(sem_recupero(1247));
    shm_remove(shm_recupero(1234, 0));
    shm_remove(shm_recupero_descrizione(1243));
    shm_remove(shm_recupero(1235, 0));
}
