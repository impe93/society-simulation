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
#include "funzioni_A.h"

pid_t pid_A = 0;
int individui_in_shm = 0;

/**
 * Handler da associare al segnale SIGTERM che svolge le dovute operazioni prima della
 * terminazione non spontanea (ordinata dal gestore) del processo A.
 */
void signal_handler(int sig);


int main(int argc, char** argv){
    /*
    Associo un handler al segnale SIGTERM, in modo da svolgere le dovute operazioni se il
    gestore sceglie di terminare il processo A.
    */
    if(signal(SIGTERM, signal_handler) == SIG_ERR){
        printf("errore in associazione a handler per segnale SIGTERM\n");
        exit(EXIT_FAILURE);
    }

    /* 
    Recupero id di: semafori, code di messaggi per comunicazione con gestore e con processi B, 
    memoria condivisa processi A.
    */
    pid_A = getpid();
    int sem_sinc_padre_id = sem_recupero(SEM_SINC_GESTORE);
    int sem_sinc_figli_id = sem_recupero(SEM_SINC_INDIVIDUI);
    int sem_shm_A = sem_recupero(SEM_SHM_A);
    int msg_gestore_A = msg_recupera_coda(MSG_GESTORE_A);
    int msg_A_B = msg_recupera_coda(MSG_A_B);
    int shm_A = shm_recupero(SHM_A_KEY, 1);
    rappresentazione_individuo* p_shm_A;
    shm_attach_rappresentazione_individuo(shm_A, &p_shm_A);

    caratteristiche_individuo individuo_A;
    int individui_B_rifiutati = 0;
    bool capostipite = FALSE;

    /*
    Recupero dei parametri passati dal gestore per la creazione (tipo, nome, genoma) dell'individuo
    */
    if(argc == 5) {
        inserimento_caratteristiche_individuo(&individuo_A, argv);
        individui_in_shm = atoi(*(argv+3));
        if(atoi(*(argv+4)) == 0 || atoi(*(argv+4)) == 1)
            capostipite = atoi(*(argv+4));
    } else {
        printf("numero di argomenti passati al processo A errato\n");
        exit(EXIT_FAILURE);
    }

    /*
    scrittura in una struct libera della shm_A dei dati rappresentanti il processo A
    dopo avvenuta sincronizzazione
    */
    inserimento_in_shm_A(p_shm_A, pid_A, individuo_A, individui_in_shm);
    sem_rilascia(sem_shm_A);

    /*
    Sincronizzazione con il gestore per inserimento in shm A delle informazioni di tutti i processi 
    creati da quest'ultimo prima di proseguire con l'esecuzione
    */
    if(capostipite){
        sem_rilascia(sem_sinc_padre_id);
        sem_riserva(sem_sinc_figli_id);
    }

    unsigned long soglia_accettazione_richiesta = individuo_A.genoma / 2;
    bool richiesta_accettata = FALSE;
    individuo_per_accoppiamento individuo_B;

    while(!richiesta_accettata){
        /*
        Legge dalla sua coda di messaggi se ha ricevuto una richiesta di accoppiamento da un B e 
        memorizza il messaggio ricevuto in una struct individuo_per_accoppiamento
        */
        msg_ricevi_messaggio_individuo(msg_A_B, pid_A, &individuo_B);
        // Valuta se accoppiarsi con B calcolando se questo può rafforzare i suoi discendenti:
        // Accetta subito se B ha genoma munltiplo del suo
        printf("PID B = %d\n", individuo_B.pid);
        printf("Genoma B = %ld\n", individuo_B.caratteristiche.genoma);
        printf("Tipo B = %c\n", individuo_B.caratteristiche.tipo);
        printf("Nome B = %s\n", individuo_B.caratteristiche.nome);
        if(individuo_B.caratteristiche.genoma % individuo_A.genoma == 0){
            richiesta_accettata = TRUE;
            DEBUG;
        } else{
            unsigned long mcd_genomi_A_B = 0;
            DEBUG;
            if (individuo_A.genoma >= individuo_B.caratteristiche.genoma) {
                mcd_genomi_A_B = mcd(individuo_A.genoma, individuo_B.caratteristiche.genoma);
            } else {
                mcd_genomi_A_B = mcd(individuo_B.caratteristiche.genoma, individuo_A.genoma);
            }
            printf("MCD = %ld\n", mcd_genomi_A_B);
            if(mcd_genomi_A_B >= soglia_accettazione_richiesta){
                richiesta_accettata = TRUE;
            } else{
                richiesta_accettata = FALSE;
            }
        }
        
        /*
        Se la richiesta è stata accettata mando una risposta di accettazione di accoppiamento a B,
        altrimenti mando una risposta di rifiuto della richiesta a B e diminuisco la standard di 
        accettazione per l'accoppiamento del processo A (se necessario).
        */
        DEBUG;
        if(richiesta_accettata){
            msg_manda_messaggio_accoppiamento(msg_A_B, TRUE, individuo_B.pid);
            DEBUG;
        } else{
            msg_manda_messaggio_accoppiamento(msg_A_B, FALSE, individuo_B.pid);
            DEBUG;
            individui_B_rifiutati++;
            if(individui_B_rifiutati == individui_in_shm){
                soglia_accettazione_richiesta = soglia_accettazione_richiesta / 2;
                individui_B_rifiutati = 0;
            }
            sem_rilascia(sem_shm_A);
        }
    }

    // Processo A comunica al gestore l'avvenuto accoppiamento con un processo B
    informazioni_accoppiamento informazioni;
    informazioni.tipo_mittente = 'A';
    informazioni.pid_mittente = pid_A;
    informazioni.pid_coniuge = individuo_B.pid;
    msg_manda_messaggio_notifica_accoppiamento(msg_gestore_A, informazioni);
    

    shm_detach_rappresentazione_individuo(p_shm_A);
    exit(EXIT_SUCCESS);
}

void signal_handler(int sig){
    int msg_A_B = msg_recupera_coda(MSG_A_B);
    int shm_A = shm_recupero(SHM_A_KEY, 1);
    int sem_shm_A = sem_recupero(SEM_SHM_A);
    rappresentazione_individuo* p_shm_A;
    individuo_per_accoppiamento individuo_B;

    shm_attach_rappresentazione_individuo(shm_A, &p_shm_A);
    /*
    se un individuo B ha cercato di contattarmi dopo la ricezione del segnale rispondo
    rifiutando la sua richiesta di accoppiamento
    */
    if(msg_controlla_presenza_messaggi(msg_A_B, pid_A, &individuo_B)){
        msg_manda_messaggio_accoppiamento(msg_A_B, FALSE, individuo_B.pid);
    }
    /*
    Rimuovo le informazioni riguardanti il processo A dalla shm A
    */
    rimozione_da_shm_A(p_shm_A, pid_A, individui_in_shm);
    sem_rilascia(sem_shm_A);
    shm_detach_rappresentazione_individuo(p_shm_A);
    exit(EXIT_SUCCESS);
}