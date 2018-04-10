#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>

#include "gestione_code.h"
#include "gestione_semafori.h"
#include "gestione_shm.h"
#include "funzioni_A.h"

int main(int argc, char** argv){

    /* 
    Recupero id di: semafori, code di messaggi per comunicazione con gestore e con processi B, 
    memoria condivisa processi A e processi B.
    Attaccamento segmento memoria condivisa A.
    */
    int sem_sinc_padre_id = sem_recupero(SEM_SINC_GESTORE);
    int sem_sinc_figli_id = sem_recupero(SEM_SINC_INDIVIDUI);
    int sem_shm_A = sem_recupero(SEM_SHM_A);
    int sem_shm_B = sem_recupero(SEM_SHM_B);
    int msg_gestore_A = recupera_coda(MSG_GESTORE_A);
    int msg_A_B = recupera_coda(MSG_A_B);
    int shm_A = shm_recupero(SHM_A_KEY, 1);
    int shm_B = shm_recupero(SHM_B_KEY, 1);
    rappresentazione_individuo** p_shm_A;
    shm_attach(shm_A, p_shm_A);

    caratteristiche_individuo individuo = NULL;
    int individui_in_shm = 0;

    /*
    Recupero dei parametri passati dal gestore per la creazione (tipo, nome, genoma) dell'individuo
    */
    if(argc == 4) {
        inserimento_caratteristiche_individuo(&individuo, argv);
        individui_in_shm = atoi(*(argv+3));
    } else {
        printf("numero di argomenti passati al processo A errato\n");
        exit(EXIT_FAILURE);
    }

    /*
    scrittura in una struct della shm_A dei dati rappresentanti il processo A libera
    dopo avvenuta sincronizzazione
    */
    sem_riserva(sem_shm_A);
    inserimento_in_shm_A(p_shm_A, getpid(), individuo);
    sem_rilascia(sem_shm_A);

    /*
    Sincronizzazione con il gestore per inserimento in shm A delle informazioni di tutti i processi 
    creati da quest'ultimo prima di proseguire con l'esecuzione
    */
    sem_rilascia(sem_sinc_padre_id);
    sem_riserva(sem_sinc_figli_id;



    





    shm_detach(p_shm_A);
    exit(EXIT_SUCCESS);
}