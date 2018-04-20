#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "tipi_simulatore_societa.h"
#include "funzioni_gestore.h"
#include "gestione_code.h"
#include "gestione_semafori.h"
#include "gestione_shm.h"

/**
* Un int che rappresenta il numero della popolazione che verrà generata all'inizio
*/
unsigned int init_people = 0;

void term_terminatore_handler (int sig) {
    exit(EXIT_SUCCESS);
}

void term_handler (int sig) {
    bool individui_terminati = FALSE;
    int sem_shm_a_id = sem_recupero(SEM_SHM_A);
    int sem_shm_b_id = sem_recupero(SEM_SHM_B);
    
    rappresentazione_individuo* individui_A;
    rappresentazione_individuo* individui_B;
    shm_attach_rappresentazione_individuo(shm_recupero(SHM_A_KEY, init_people), &individui_A);
    shm_attach_rappresentazione_individuo(shm_recupero(SHM_B_KEY, init_people), &individui_B);
    while(individui_terminati == FALSE) {
        sem_riserva(sem_shm_a_id);
        sem_riserva(sem_shm_b_id);
        int numero_individui_A = conta_individui_attivi(individui_A, init_people);
        int numero_individui_B = conta_individui_attivi(individui_B, init_people);
        if (numero_individui_A == 0 && numero_individui_B == 0) {
            individui_terminati = TRUE;
        }
        sem_rilascia(sem_shm_a_id);
        sem_rilascia(sem_shm_b_id);
    }
    while(wait(NULL) != -1);

    msg_rimuovi_coda(msg_recupera_coda(1240));
    msg_rimuovi_coda(msg_recupera_coda(1241));
    msg_rimuovi_coda(msg_recupera_coda(1242));
    sem_cancella(sem_recupero(1236));
    sem_cancella(sem_recupero(1237));
    sem_cancella(sem_recupero(1238));
    sem_cancella(sem_recupero(1239));
    sem_cancella(sem_recupero(1244));
    shm_remove(shm_recupero(1234, 12));
    shm_remove(shm_recupero_descrizione(1243));
    shm_remove(shm_recupero(1235, 12));
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    
    if (signal(SIGTERM, term_handler) == (void*)-1) {
        printf("Errore durante l'assegnamento dell'handler al gestore.\n");
        exit(EXIT_FAILURE);
    }

    // Utilizzato e chiamato solo una volta per generare dei numeri casuali
    srand(time(NULL));

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

    /**
     * Il pid del gestore
     */
    pid_t pid_gestore = getpid();

    /**
     * Il pid del terminatore di processi che appartiene al gestore
     */
    pid_t pid_terminatore_processi = 0;

    /**
     * Il puntatore alla struttura contenente i dati della simulazione
     */
    descrizione_simulazione* descrizione = NULL;

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
    int shm_a_id = shm_creazione(SHM_A_KEY, init_people - 1);
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
     * Inizializzazione della shared memory per l'accesso alla descrizione della simulazione
     * ed attaccamento della variabile alla memoria.
     */
    int shm_descrizione_id = shm_creazione_descrizione(SHM_DESCRIZIONE_KEY);
    shm_attach_descrizione_simulazione(shm_descrizione_id, &descrizione);

    /**
     * 
     */
    int sem_shm_descrizione_id = sem_creazione(SEM_SHM_DESCRIZIONE);
    sem_init_disponibile(sem_shm_descrizione_id);
    

    /**
     * Inizializzazione e sincronizzazione degli individui
     */
    inizializza_individui(init_people, genes);
    for (int i = 0; i < init_people; i++) {
        sem_riserva(sem_sinc_padre_id);
    }
    for(int i = 0; i < init_people; i++) {
        sem_rilascia(sem_sinc_figli_id);
    }

    /**
     * Creazione del figlio che si occupa della terminazione casuale dei processi A e B.
     */
    switch(pid_terminatore_processi = fork()) {
        case -1: {
            printf("Errore durante la creazione del figlio che si occupa della terminazione dei processi A e B.\n");
            exit(EXIT_FAILURE);
        }
        case 0: {
            if (signal(SIGTERM, term_terminatore_handler) == (void*)-1) {
                printf("Errore durante l'assegnazione dell'handler al terminatore di processi.\n");
                exit(EXIT_FAILURE);
            }
            attivita_terminatore_individui(init_people, birth_death, genes, descrizione);
            exit(EXIT_SUCCESS);
        }
        default: break;
    }

    /**
     * Creazione del figlio che fa il conto alla rovescia per la terminazione.
     */
    switch(fork()) {
        case -1: {
            printf("Errore durante la creazione del processo per segnalazione time-out.\n");
            exit(EXIT_FAILURE);
        }
        case 0: {
            terminazione_simulazione(sim_time, init_people, pid_gestore, pid_terminatore_processi);
            exit(EXIT_SUCCESS);
        }
        default: break;
    }

    while(wait(NULL) != -1);
    
}