#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "tipi_simulatore_societa.h"
#include "funzioni_gestore.h"
#include "gestione_code.h"
#include "gestione_semafori.h"
#include "gestione_shm.h"

int main(int argc, char** argv) {
    
    // Utilizzato e chiamato solo una volta per generare dei numeri casuali
    srand(time(NULL));

    /**
     * Un int che rappresenta il numero della popolazione che verrà generata all'inizio
     */
    unsigned int init_people = 0;

    /**
     * Un unsigned long che rappresenta il gene
     */
    unsigned long genes = 0;

    /**
     * Un int che rappresenta i secondi che devono passare prima che un processo debba
     * terminare
     */
    unsigned int birth_death = 0;

    /**
     * Un int che rappresenta i secondi di esecuzione della simulazione.
     */
    unsigned int sim_time = 0;

    // Assegnamento delle variabili passate come parametro
    if (argc == 5) {
        bool corretto = TRUE;

        if (isUnsignedNumber(*(argv + 1))) {
            init_people = atoi(*(argv + 1));
        } else {
            printf("Il numero che rappresenta la popolazione deve essere un intero non segnato.\n");
            corretto = FALSE;
        }

        if (isUnsignedNumber(*(argv + 2))) {
            genes = atol(*(argv + 2));
        } else {
            printf("Il numero che rappresenta il gene deve essere un long non segnato.\n");
            corretto = FALSE;
        }

        if (isUnsignedNumber(*(argv + 3))) {
            birth_death = atol(*(argv + 3));
        } else {
            printf("Il numero che rappresenta il tempo che ci mette a morire un popolano dev'essere un intero.\n");
            corretto = FALSE;
        }

        if (isUnsignedNumber(*(argv + 4))) {
            sim_time = atoi(*(argv + 4));
        } else {
            printf("Il numero che rappresenta la durata della simulazione dev'essere un intero.\n");
            corretto = FALSE;
        }

        if (!corretto) {
            exit(EXIT_FAILURE);
        }

    } else {
        printf("Il numero di parametri non è corretto.\nInserire in quest'ordine: init_people genes birth_death sim_time\n");
        exit(EXIT_FAILURE);
    }

    /**
     * l'ID della shm creata per gli individui di tipo A
     */
    int shm_a_id = shm_creazione(SHM_A_KEY, init_people);
    inizializza_shm(shm_a_id, init_people);


    /**
     * Creazione e inizializzazione del semaforo per l'accesso alla shm per
     * individui di tipo A
     */
    int sem_shm_a_id = sem_creazione(SEM_SHM_A);
    sem_init_disponibile(sem_shm_a_id);

    /**
     * l'ID della shm creata per gli individui di tipo B
     */
    int shm_b_id = shm_creazione(SHM_B_KEY, init_people - 1);
    inizializza_shm(shm_b_id, init_people);
    
    /**
     * Creazione e inizializzazione del semaforo per l'accesso alla shm per
     * individui di tipo A
     */
    int sem_shm_b_id = sem_creazione(SEM_SHM_B);
    sem_init_disponibile(sem_shm_b_id);

    /**
     * ID ed inizializzazione dei semafori per la sincronizzazione
     * degli individui prima della partenza con il gestore
     */
    int sem_sinc_padre_id = sem_creazione(SEM_SINC_GESTORE);
    sem_init_occupato(sem_sinc_padre_id);
    
    int sem_sinc_figli_id = sem_creazione(SEM_SINC_INDIVIDUI);
    sem_init_occupato(sem_sinc_figli_id);

    /**
     * Crea una coda di messaggi per la comunicazione tra i processi A e B
     */
    int msg_a_b_id = msg_crea_coda_messaggi(MSG_A_B);

    /**
     * Crea una coda di messaggi per la comunicazione tra gestore ed A
     */
    int msg_gestore_a = msg_crea_coda_messaggi(MSG_GESTORE_A);

    /**
     * Crea una coda di messaggi per la comunicazione tra gestore ed B
     */
    int msg_gestore_b = msg_crea_coda_messaggi(MSG_GESTORE_B);

    /**
     * Inizializzazione e sincronizzazione degli individui
     */
    inizializza_individui(init_people, genes);
    for (int i = 0; i < init_people; i++) {
        printf("init_people = %i, i = %i\n", init_people, i);
        sem_riserva(sem_sinc_padre_id);
    }
    sem_cancella(sem_sinc_padre_id);
    for(int i = 0; i < init_people; i++) {
        sem_rilascia(sem_sinc_figli_id);
    }



}