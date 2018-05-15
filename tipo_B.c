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
#include "funzioni_A_B.h"
#include "funzioni_B.h"

/**
 * PID associato all'individuo A.
 */
pid_t pid_B = 0;

/**
 * Numero di individui presenti nella simulazione.
 */
int individui_in_shm = 0;

/**
 * Handler da associare al segnale SIGTERM che svolge le dovute operazioni prima della
 * terminazione non spontanea (ordinata dal gestore) del processo B.
 */
void signal_handler(int sig);

int main (int argc, char** argv) {

  /*
  Associo un handler al segnale SIGTERM, in modo da svolgere le dovute operazioni se il
  gestore sceglie di terminare il processo B.
  */
  if(signal(SIGTERM, signal_handler) == SIG_ERR){
    printf("errore in associazione a handler per segnale SIGTERM\n");
    exit(EXIT_FAILURE);
  }

  /**
   * Recupero id di: semafori, code di messaggi per comunicazione con gestore e con processi B, 
   * memoria condivisa processi A.
   */
  pid_B = getpid();
  int sem_sinc_padre_id = sem_recupero(SEM_SINC_GESTORE);
  int sem_azione_b_id = sem_recupero(SEM_AZIONE_B);
  int sem_azione_a_id = sem_recupero(SEM_AZIONE_A);
  int sem_azione_id = sem_recupero(SEM_AZIONE);
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

  /**
   * Contiene tutte le informazioni sull'individuo B associato a questo processo.
   */
  caratteristiche_individuo individuo_B;

  /**
   * Contiene tutte le informazioni sull'individuo A al quale è stata inviata l'ultima
   * richiesta di accoppiamento.
   */
  individuo_per_accoppiamento individuo_A;

  /**
   * Indica quali operazioni iniziali deve effettuare il processo A.
   */
  int comportamento_iniziale = 0;

  bool richiesta_accettata = FALSE;

  /**
   * Recupero dei parametri passati dal gestore per la creazione (tipo, nome, genoma) dell'individuo, 
   * per la memorizzazione del numero di individui presenti in shm (individui_in_shm),
   * per l'effettuazione delle corrette operazioni iniziali (comportamento_iniziale).
   */
  if(argc == 5) {
    inserimento_caratteristiche_individuo(&individuo_B, argv);
    individui_in_shm = atoi(*(argv+3));
    if(atoi(*(argv+4)) == 0 || atoi(*(argv+4)) == 1 || atoi(*(argv+4)) == 2)
      comportamento_iniziale = atoi(*(argv+4));
  } else {
    printf("numero di argomenti passati al processo B errato\n");
    exit(EXIT_FAILURE);
  }

  /**
   * Scrittura in una rappresentazione_individuo libera della shm_B dei dati rappresentanti l'individuo B
   * e successivo rilascio del semaforo per l'accesso in mutua esclusione alla shm B.
  */
  inserimento_in_shm(p_shm_B, pid_B, individuo_B, individui_in_shm);
  sem_rilascia(sem_shm_B);
  
  /**
   * Sincronizzazione iniziale con il gestore per inserimento in shm delle informazioni di tutti i
   * processi oppure effettuazione delle dovute operazioni per la terminazione di una macro azione.
   */
  switch(comportamento_iniziale) {

    case 0: {
      sem_rilascia(sem_sinc_padre_id);
      sem_riserva(sem_sinc_figli_id);
      break;
    }
    case 1: {
      sem_rilascia(sem_azione_id);
      break;
    }
    case 2: {
      sem_rilascia(sem_azione_b_id);
      break;
    }
    default: {
      printf("Errore durante la scelta del comportamento iniziale.\n");
      exit(EXIT_FAILURE);
    }

  }


  while(!richiesta_accettata){

    /**
     * Inizia la fase di accoppiamento.
     */
    sem_riserva(sem_azione_id);
    sem_riserva(sem_azione_a_id);
    sem_riserva(sem_azione_b_id);
    sem_riserva(sem_shm_A);
    sem_riserva(sem_shm_B);

    /**
     * Sceglie per l'accoppiamento il processo A col quale può generare un figlio con genoma
     * più alto possibile.
     */
    seleziona_individuo_A_ideale(p_shm_A, &individuo_A, individuo_B.genoma, individui_in_shm);
    
    /**
     * Manda all'individuo A scelto in precedenza un messaggio contenente la sua richiesta
     * di accoppiamento.
     */
    individuo_per_accoppiamento messaggio_accoppiamento;
    inserimento_in_messaggio_accoppiamento(&messaggio_accoppiamento, pid_B, individuo_B);
    msg_manda_messaggio_individuo(msg_A_B, messaggio_accoppiamento, individuo_A.pid);

    /**
     * Rimane in attesa finchè non riceve una risposta alla sua richiesta di accoppiamento
     * dall'individuo A che ha contattato.
     */
    msg_ricevi_messaggio_accoppiamento(msg_A_B, pid_B, &richiesta_accettata);

    /**
     * Se l'individuo A ha rifiutato la richiesta di accoppiamento allora termina la fase
     * di accoppiamento liberando le risorse ancora da rilasciare.
     */
    if(!richiesta_accettata){
      sem_rilascia(sem_azione_id);
      sem_rilascia(sem_shm_B);
      sem_rilascia(sem_azione_b_id);
    }

  }

  /** 
   * L'individuo B notifica al gestore l'avvenuto accoppiamento con un individuo A inviandogli
   * un messaggio sulla coda di comunicazione tra gestore e processi B.
   */
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

  /** 
   * Rimuovo le informazioni riguardanti il processo B dalla shm B e poi rilascio il semaforo 
   * per accesso in mutua esclusione nella shm B riservato dal gestore prima della terminazione.
   */
  rimozione_da_shm(p_shm_B, pid_B, individui_in_shm);
  sem_rilascia(sem_shm_B);
  shm_detach_rappresentazione_individuo(p_shm_B);
  
  exit(EXIT_SUCCESS);

}
