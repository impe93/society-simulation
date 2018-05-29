#include "gestione_semafori.h"

int sem_creazione(int chiave) {
  int id = 0;
  if((id = semget(chiave, 1, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
    printf("Erorre durante la creazione del semaforo con ID = %i.\n", chiave);
    exit(EXIT_FAILURE);
  }
  return id;
}

int sem_recupero(int chiave) {
  int id = 0;
  if((id = semget(chiave, 1, 0)) == -1) {
    printf("Erorre durante il recupero del semaforo con ID = %i.\n", chiave);
    exit(EXIT_FAILURE);
  }
  return id;
}

void sem_init_disponibile(int id) {
  if (initSemAvailable(id, 0) == -1) {
    printf("Errore durante l'inizializzazione a disponibile del semaforo con ID = %i.\n", id);
    exit(EXIT_FAILURE);
  }
}

void sem_init_occupato(int id) {
  if (initSemInUse(id, 0) == -1) {
    printf("Errore durante l'inizializzazione a occupato del semaforo con ID = %i.\n", id);
    exit(EXIT_FAILURE);
  }
}

void sem_riserva(int id) {
  if (reserveSem(id, 0) == -1) {
    printf("Errore durante la riserva del semaforo con ID = %i e PID = %i.\n", id, getpid());
    exit(EXIT_FAILURE);
  }
}

void sem_rilascia(int id) {
  if (releaseSem(id, 0) == -1) {
    printf("Errore durante il rilascio del semaforo con ID = %i.\n", id);
    exit(EXIT_FAILURE);
  }
}

void sem_cancella(int id) {
  if (semctl(id, 0, IPC_RMID) == -1) {
    printf("Errore durante la rimozione del semaforo con ID = %i.\n", id);
    exit(EXIT_FAILURE);
  }
}
