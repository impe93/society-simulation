#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>

#include "tipi_simulatore_societa.h"
#include "gestione_code.h"
#include "gestione_semafori.h"
#include "gestione_shm.h"
#include "funzioni_B.h"

pid_t pid_B = 0;
int individui_in_shm = 0;

/**
 * Handler da associare al segnale SIGTERM che svolge le dovute operazioni prima della
 * terminazione non spontanea (ordinata dal gestore) del processo B.
 */
void signal_handler(int sig);

int main(int argc, char** argv){
    /*
    Associo un handler al segnale SIGTERM, in modo da svolgere le dovute operazioni se il
    gestore sceglie di terminare il processo B.
    */
    if(signal(SIGTERM, signal_handler) == SIG_ERR){
        printf("errore in associazione a handler per segnale SIGTERM\n");
        exit(EXIT_FAILURE);
    }

    /* 
    Recupero id di: semafori, code di messaggi per comunicazione con gestore e con processi A, 
    memoria condivisa processi A e processi B.
    */
    pid_B = getpid();
    int sem_sinc_padre_id = sem_recupero(SEM_SINC_GESTORE);
    int sem_sinc_figli_id = sem_recupero(SEM_SINC_INDIVIDUI);
    int sem_shm_A = sem_recupero(SEM_SHM_A);
    int sem_shm_B = sem_recupero(SEM_SHM_B);
    int msg_gestore_B = msg_recupera_coda(MSG_GESTORE_B);
    int msg_A_B = msg_recupera_coda(MSG_A_B);
    int shm_A = shm_recupero(SHM_A_KEY, 1);
    int shm_B = shm_recupero(SHM_B_KEY, 1);
    rappresentazione_individuo* p_shm_A;
    rappresentazione_individuo* p_shm_B;
    shm_attach_rappresentazione_individuo(shm_A, &p_shm_A);
    shm_attach_rappresentazione_individuo(shm_B, &p_shm_B);

    caratteristiche_individuo individuo_B;
    individuo_per_accoppiamento individuo_A;
    bool richiesta_accettata = FALSE;
    bool capostipite = FALSE;

    /*
    Recupero dei parametri passati dal gestore per la creazione (tipo, nome, genoma) dell'individuo
    */
    if(argc == 5) {
        inserimento_caratteristiche_individuo(&individuo_B, argv);
        individui_in_shm = atoi(*(argv+3));
        if(atoi(*(argv+4)) == 0 || atoi(*(argv+4)) == 1)
            capostipite = atoi(*(argv+4));
    } else {
        printf("numero di argomenti passati al processo B errato\n");
        exit(EXIT_FAILURE);
    }

    /*
    scrittura in una struct libera della shm_B dei dati rappresentanti il processo B
    dopo avvenuta sincronizzazione
    */
    inserimento_in_shm_B(p_shm_B, pid_B, individuo_B, individui_in_shm);
    sem_rilascia(sem_shm_B);

    /*
    Sincronizzazione con il gestore per inserimento in shm B delle informazioni di tutti i processi 
    creati da quest'ultimo prima di proseguire con l'esecuzione
    */
   if(capostipite){
        sem_rilascia(sem_sinc_padre_id);
        sem_riserva(sem_sinc_figli_id);
    }

    while(!richiesta_accettata){
        sem_riserva(sem_shm_A);
        sem_riserva(sem_shm_B);
        
        seleziona_individuo_A_ideale(p_shm_A, &individuo_A, individuo_B.genoma, individui_in_shm);
        
        individuo_per_accoppiamento messaggio_accoppiamento;
        inserimento_in_messaggio_accoppiamento(&messaggio_accoppiamento, pid_B, individuo_B);
        msg_manda_messaggio_individuo(msg_A_B, messaggio_accoppiamento, individuo_A.pid);

        msg_ricevi_messaggio_accoppiamento(msg_A_B, pid_B, &richiesta_accettata);

        if(!richiesta_accettata){
            sem_rilascia(sem_shm_B);
        }
    }

    //processo B comunica al gestore l'avvenuto accoppiamento con un processo A
    informazioni_accoppiamento informazioni;
    informazioni.tipo_mittente = 'B';
    informazioni.pid_mittente = pid_B;
    informazioni.pid_coniuge = individuo_A.pid;
    msg_manda_messaggio_notifica_accoppiamento(msg_gestore_B, informazioni);

    shm_detach_rappresentazione_individuo(p_shm_A);
    shm_detach_rappresentazione_individuo(p_shm_B);
    exit(EXIT_SUCCESS);
}

void signal_handler(int sig){
    
    int shm_B = shm_recupero(SHM_B_KEY, 1);
    int sem_shm_B = sem_recupero(SEM_SHM_B);
    rappresentazione_individuo* p_shm_B;

    shm_attach_rappresentazione_individuo(shm_B, &p_shm_B);

    /*
    Rimuovo le informazioni riguardanti il processo B dalla shm B
    */
    rimozione_da_shm_B(p_shm_B, pid_B, individui_in_shm);

    sem_rilascia(sem_shm_B);
    shm_detach_rappresentazione_individuo(p_shm_B);

    exit(EXIT_SUCCESS);
}