#include "gestione_shm.h"

int shm_creazione(int chiave, int grandezza_array) {
    int id = 0;
    if ((id = shmget(chiave, sizeof(rappresentazione_individuo) * grandezza_array, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        printf("Errore durante la creazione della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

int shm_creazione_descrizione(int chiave) {
    int id = 0;
    if ((id = shmget(chiave, sizeof(descrizione_simulazione), IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        printf("Errore durante la creazione della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

int shm_recupero(int chiave, int grandezza_array) {
    int id = 0;
    if ((id = shmget(chiave, sizeof(rappresentazione_individuo) * grandezza_array, 0)) == -1) {
        printf("Errore durante il recupero della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

int shm_recupero_descrizione(int chiave) {
    int id = 0;
    if ((id = shmget(chiave, sizeof(descrizione_simulazione), 0)) == -1) {
        printf("Errore durante la creazione della memoria condivisa con chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

void shm_attach_rappresentazione_individuo(int shmid, rappresentazione_individuo** p) {
    if(((*p) = (rappresentazione_individuo*) shmat(shmid, NULL, 0)) == (void*)-1) {
        ERRNO;
        printf("errore in attacco del segmento di shm con id %i nel pid %i\n", shmid, getpid());
        exit(EXIT_FAILURE);
    }
}

void shm_attach_descrizione_simulazione(int shmid, descrizione_simulazione** p) {
    if(((*p) = (descrizione_simulazione*) shmat(shmid, NULL, 0)) == (void*)-1) {
        ERRNO;
        printf("errore in attacco del segmento di shm con id %i nel pid %i\n", shmid, getpid());
        exit(EXIT_FAILURE);
    }
}

void shm_detach_rappresentazione_individuo(rappresentazione_individuo* shmaddr){
    if(shmdt(shmaddr) == -1){
        printf("Errore durante il distacco della shm\n");
        exit(EXIT_FAILURE);
    }
}

void shm_detach_descrizione_simulazione(descrizione_simulazione* shmaddr) {
    if(shmdt(shmaddr) == -1){
        printf("Errore durante il distacco della shm\n");
        exit(EXIT_FAILURE);
    }
}

void shm_remove(int shmid){
    if(shmctl(shmid, IPC_RMID, 0) == -1){
        printf("errore nella rimozione del frammento di shm\n");
        exit(EXIT_FAILURE);
    }
}