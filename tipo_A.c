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

/**
 * PID associato all'individuo A.
 */
pid_t pid_A = 0;

/**
 * Numero di individui presenti nella simulazione.
 */
int individui_in_shm = 0;

/**
 * Handler da associare al segnale SIGTERM che svolge le dovute operazioni prima della
 * terminazione non spontanea (ordinata dal gestore) del processo A.
 */
void signal_handler(int sig);

int main (int argc, char** argv) {

  /**
   * Associo un handler al segnale SIGTERM, in modo da svolgere le dovute operazioni se il
   * gestore sceglie di terminare il processo A.
   */
  if(signal(SIGTERM, signal_handler) == SIG_ERR){
    printf("errore in associazione a handler per segnale SIGTERM\n");
    exit(EXIT_FAILURE);
  }

  /**
   * Recupero id di: semafori, code di messaggi per comunicazione con gestore e con processi B, 
   * memoria condivisa processi A.
   */
  pid_A = getpid();
  int sem_sinc_padre_id = sem_recupero(SEM_SINC_GESTORE);
  int sem_sinc_figli_id = sem_recupero(SEM_SINC_INDIVIDUI);
  int sem_azione_a_id = sem_recupero(SEM_AZIONE_A);
  int sem_azione_id = sem_recupero(SEM_AZIONE);
  int sem_shm_A = sem_recupero(SEM_SHM_A);
  int msg_gestore_A = msg_recupera_coda(MSG_GESTORE_A);
  int msg_A_B = msg_recupera_coda(MSG_A_B);
  int shm_A = shm_recupero(SHM_A_KEY, 1);
  rappresentazione_individuo* p_shm_A;
  shm_attach_rappresentazione_individuo(shm_A, &p_shm_A);

  /**
   * Contiene tutte le informazioni sull'individuo A associato a questo processo.
   */
  caratteristiche_individuo individuo_A;

  /**
   * Indica il numero di individui B a cui l'individuo A ha rifiutato una richiesta di accoppiamento.
   */
  int individui_B_rifiutati = 0;
  
  /**
   * Indica quali operazioni iniziali deve effettuare il processo A.
   */
  int comportamento_iniziale = 0;

  /**
   * Valore che rappresenta la soglia che il genoma di un individuo B deve possedere affinchè
   * la sua richiesta di accoppiamento venga accettata.
   */
  unsigned long soglia_accettazione_richiesta = individuo_A.genoma / 2;

  /**
   * Contiene tutte le informazioni sull'individuo B dal quale è stata ricevuta l'ultima
   * richiesta di accoppiamento.
   */
  individuo_per_accoppiamento individuo_B;

  bool richiesta_accettata = FALSE;

  /**
   * Recupero dei parametri passati dal gestore per la creazione (tipo, nome, genoma) dell'individuo, 
   * per la memorizzazione del numero di individui presenti in shm (individui_in_shm),
   * per l'effettuazione delle corrette operazioni iniziali (comportamento_iniziale).
   */
  if(argc == 5) {
    inserimento_caratteristiche_individuo(&individuo_A, argv);
    individui_in_shm = atoi(*(argv+3));
    if(atoi(*(argv+4)) == 0 || atoi(*(argv+4)) == 1 || atoi(*(argv+4)) == 2)
      comportamento_iniziale = atoi(*(argv+4));
  } else {
    printf("numero di argomenti passati al processo A errato\n");
    exit(EXIT_FAILURE);
  }

  /**
   * Scrittura in una rappresentazione_individuo libera della shm_A dei dati rappresentanti l'individuo A
   * e successivo rilascio del semaforo per l'accesso in mutua esclusione alla shm A.
  */
  inserimento_in_shm(p_shm_A, pid_A, individuo_A, individui_in_shm);
  sem_rilascia(sem_shm_A);

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
      sem_rilascia(sem_azione_a_id);
      break;
    }
    default: {
      printf("Errore durante la scelta del comportamento iniziale.\n");
      exit(EXIT_FAILURE);
    }

  }

  while(!richiesta_accettata){

    /** 
     * Rimane in attesa finchè non riceve una richiesta di accoppiamento da un B sulla 
     * coda di messaggi per la comunicazione con i processi B, a questo punto memorizza 
     * i dati dell'individuo B in individuo_B.
     */
    msg_ricevi_messaggio_individuo(msg_A_B, pid_A, &individuo_B);

    /** 
     * Valuta se accoppiamento con B può rafforzare i suoi discendenti e in caso 
     * positivo accetta la richiesta.
     */
    if(individuo_B.caratteristiche.genoma % individuo_A.genoma == 0){
      richiesta_accettata = TRUE;
    } else{
      unsigned long mcd_genomi_A_B = 0;
      if (individuo_A.genoma >= individuo_B.caratteristiche.genoma) {
        mcd_genomi_A_B = mcd(individuo_A.genoma, individuo_B.caratteristiche.genoma);
      } else {
        mcd_genomi_A_B = mcd(individuo_B.caratteristiche.genoma, individuo_A.genoma);
      }
      if(mcd_genomi_A_B >= soglia_accettazione_richiesta){
        richiesta_accettata = TRUE;
      } else{
        richiesta_accettata = FALSE;
      }
    }
    
    /** 
     * Se la richiesta è stata accettata comunica a B l'accettazione dell'accoppiamento; altrimenti
     * gli comunica il suo rifiuto, diminuisce il suo standard di accettazione per l'accoppiamento
     * (se necessario) e rilascia il semaforo della shm A e quello dell'azione di un processo A.
     */
    if(richiesta_accettata){
      msg_manda_messaggio_accoppiamento(msg_A_B, TRUE, individuo_B.pid);
    } else {
      msg_manda_messaggio_accoppiamento(msg_A_B, FALSE, individuo_B.pid);
      individui_B_rifiutati++;
      if(individui_B_rifiutati == (individui_in_shm / 2)){
        soglia_accettazione_richiesta = soglia_accettazione_richiesta / 2;
        individui_B_rifiutati = 0;
      }
      sem_rilascia(sem_shm_A);
      sem_rilascia(sem_azione_a_id);
    }
  }

  /** 
   * L'individuo A notifica al gestore l'avvenuto accoppiamento con un individuo B inviandogli
   * un messaggio sulla coda di comunicazione tra gestore e processi A.
   */
  informazioni_accoppiamento informazioni;
  informazioni.tipo_mittente = 'A';
  informazioni.pid_mittente = pid_A;
  informazioni.pid_coniuge = individuo_B.pid;
  msg_manda_messaggio_notifica_accoppiamento(msg_gestore_A, informazioni);
  
  shm_detach_rappresentazione_individuo(p_shm_A);

  exit(EXIT_SUCCESS);

}

void signal_handler (int sig) {

  int msg_A_B = msg_recupera_coda(MSG_A_B);
  int shm_A = shm_recupero(SHM_A_KEY, 1);
  int sem_shm_A = sem_recupero(SEM_SHM_A);
  rappresentazione_individuo* p_shm_A;
  individuo_per_accoppiamento individuo_B;

  shm_attach_rappresentazione_individuo(shm_A, &p_shm_A);
  
  /** 
   * Rimuovo le informazioni riguardanti il processo A dalla shm A e poi rilascio il semaforo 
   * per accesso in mutua esclusione nella shm A riservato dal gestore prima dell'invio
   * dell'ordine di terminazione.
   */
  rimozione_da_shm(p_shm_A, pid_A, individui_in_shm);
  sem_rilascia(sem_shm_A);
  shm_detach_rappresentazione_individuo(p_shm_A);

  exit(EXIT_SUCCESS);

}
