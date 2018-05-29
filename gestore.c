#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include "tipi_simulatore_societa.h"
#include "funzioni_gestore.h"
#include "gestione_code.h"
#include "gestione_semafori.h"
#include "gestione_shm.h"

unsigned int init_people = 0;

void term_handler (int sig) {
  bool individui_terminati = FALSE;
  int sem_shm_a_id = sem_recupero(SEM_SHM_A);
  int sem_shm_b_id = sem_recupero(SEM_SHM_B);
  int sem_azione_id = sem_recupero(SEM_AZIONE);
  int sem_azione_b_id = sem_recupero(SEM_AZIONE_B);
  int sem_azione_a_id = sem_recupero(SEM_AZIONE_A);
  
  rappresentazione_individuo* individui_A;
  rappresentazione_individuo* individui_B;
  shm_attach_rappresentazione_individuo(shm_recupero(SHM_A_KEY, init_people - 1), &individui_A);
  shm_attach_rappresentazione_individuo(shm_recupero(SHM_B_KEY, init_people - 1), &individui_B);
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

  aggiorna_utente_terminazione_simulazione();

  sem_rilascia(sem_azione_id);
  sem_rilascia(sem_azione_a_id);
  sem_rilascia(sem_azione_b_id);

  msg_rimuovi_coda(msg_recupera_coda(MSG_A_B));
  msg_rimuovi_coda(msg_recupera_coda(MSG_GESTORE_A));
  msg_rimuovi_coda(msg_recupera_coda(MSG_GESTORE_B));
  sem_cancella(sem_recupero(SEM_SINC_GESTORE));
  sem_cancella(sem_recupero(SEM_SINC_INDIVIDUI));
  sem_cancella(sem_shm_a_id);
  sem_cancella(sem_shm_b_id);
  sem_cancella(sem_recupero(SEM_SHM_DESCRIZIONE));
  sem_cancella(sem_azione_id);
  sem_cancella(sem_azione_a_id);
  sem_cancella(sem_azione_b_id);
  shm_remove(shm_recupero(SHM_A_KEY, init_people - 1));
  shm_remove(shm_recupero_descrizione(SHM_DESCRIZIONE_KEY));
  shm_remove(shm_recupero(SHM_B_KEY, init_people - 1));
  exit(EXIT_SUCCESS);
}

void term_terminatore_handler (int sig) {
  exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
  
  if (signal(SIGTERM, term_handler) == SIG_ERR) {
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
   * Un int che rappresenta i secondi di esecuzione della simulazione
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

  /**
   * Il puntatore alla memoria condivisa dove sono contenuti gli individui A
   */
  rappresentazione_individuo* individui_A;

  /**
   * Il puntatore alla memoria condivisa dove sono contenuti gli individui B
   */
  rappresentazione_individuo* individui_B;
  
  // Assegnamento delle variabili passate come parametro
  if (argc == 5) {
    bool corretto = TRUE;

    if (isUnsignedNumber(*(argv + 1))) {
      init_people = atoi(*(argv + 1));
      if (init_people < 2 || init_people > 300) {
        printf("Il numero che rappresenta la popolazione deve essere un intero senza segno compreso tra 2 e 300.\n");
        corretto = FALSE;
      }
    } else {
      printf("Il numero che rappresenta la popolazione deve essere un intero senza segno compreso tra 2 e 300.\n");
      corretto = FALSE;
    }

    if (isUnsignedNumber(*(argv + 2))) {
      genes = atol(*(argv + 2));
      printf("Genes = %lu\n", genes);
      printf("LONG_MAX   :   %lu\n", (unsigned long) ULONG_MAX);
    } else {
      printf("Il numero che rappresenta il genes deve essere un long senza segno.\n");
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
   * Semaforo che regolamenta le macro azioni (accoppiamento, terminazione
   * processi e terminazione simulazione)
   */
  int sem_azione_id = sem_creazione(SEM_AZIONE);
  sem_init_disponibile(sem_azione_id);

  /**
   * Semaforo che regolamenta le azioni per gli individui A
   */
  int sem_azione_a_id = sem_creazione(SEM_AZIONE_A);
  sem_init_disponibile(sem_azione_a_id);

  /**
   * Semaforo che regolamenta le azioni per gli individui B
   */
  int sem_azione_b_id = sem_creazione(SEM_AZIONE_B);
  sem_init_disponibile(sem_azione_b_id);

  /**
   * L'ID della shm creata per gli individui di tipo A
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
   * L'ID della shm creata per gli individui di tipo B
   */
  int shm_b_id = shm_creazione(SHM_B_KEY, init_people - 1);
  inizializza_shm(shm_b_id, init_people);
  
  /**
   * Creazione e inizializzazione del semaforo per l'accesso alla shm per
   * individui di tipo B
   */
  int sem_shm_b_id = sem_creazione(SEM_SHM_B);
  sem_init_disponibile(sem_shm_b_id);

  /**
   * ID ed inizializzazione dei semafori per la sincronizzazione
   * degli individui con il gestore prima dell'avvio della simulazione
   */
  int sem_sinc_padre_id = sem_creazione(SEM_SINC_GESTORE);
  sem_init_occupato(sem_sinc_padre_id);

  int sem_sinc_figli_id = sem_creazione(SEM_SINC_INDIVIDUI);
  sem_init_occupato(sem_sinc_figli_id);

  msg_crea_coda_messaggi(MSG_A_B);

  /**
   * Crea una coda di messaggi per la comunicazione tra gestore ed individui A
   */
  int msg_gestore_a = msg_crea_coda_messaggi(MSG_GESTORE_A);

  /**
   * Crea una coda di messaggi per la comunicazione tra gestore ed individui B
   */
  int msg_gestore_b = msg_crea_coda_messaggi(MSG_GESTORE_B);

  /**
   * Inizializzazione della shared memory per l'accesso alla descrizione della simulazione
   * ed attaccamento della variabile alla memoria.
   */
  int shm_descrizione_id = shm_creazione_descrizione(SHM_DESCRIZIONE_KEY);
  shm_attach_descrizione_simulazione(shm_descrizione_id, &descrizione);

  /**
   * Inizializzazione del semaforo per la creazione
   */
  int sem_shm_descrizione_id = sem_creazione(SEM_SHM_DESCRIZIONE);
  sem_init_disponibile(sem_shm_descrizione_id);
  
  /**
   * Attaccamento della shm
   */
  shm_attach_rappresentazione_individuo(shm_a_id, &individui_A);
  shm_attach_rappresentazione_individuo(shm_b_id, &individui_B);

  inizializza_individui(init_people, genes);
  for (int i = 0; i < init_people; i++) {
    sem_riserva(sem_sinc_padre_id);
  }
  for(int i = 0; i < init_people; i++) {
    sem_rilascia(sem_sinc_figli_id);
  }

  /**
   * Creazione del figlio che si occupa della terminazione casuale dei processi A e B
   */
  switch(pid_terminatore_processi = fork()) {
    case -1: {
      printf("Errore durante la creazione del figlio che si occupa della terminazione dei processi A e B.\n");
      exit(EXIT_FAILURE);
    }
    case 0: {
      if (signal(SIGTERM, term_terminatore_handler) == SIG_ERR) {
        printf("Errore durante l'assegnazione dell'handler al terminatore di processi.\n");
        exit(EXIT_FAILURE);
      }
      attivita_terminatore_individui(init_people, birth_death, genes, descrizione);
      exit(EXIT_SUCCESS);
    }
    default: break;
  }

  /**
   * Creazione del figlio che fa il conto alla rovescia per la terminazione della simulazione
   */
  switch(fork()) {
    case -1: {
      printf("Errore durante la creazione del processo per segnalazione time-out.\n");
      exit(EXIT_FAILURE);
    }
    case 0: {
      if (signal(SIGTERM, term_terminatore_handler) == SIG_ERR) {
        printf("Errore durante l'assegnazione dell'handler al terminatore di processi.\n");
        exit(EXIT_FAILURE);
      }
      terminazione_simulazione(sim_time, init_people, pid_gestore, pid_terminatore_processi);
      exit(EXIT_SUCCESS);
    }
    default: break;
  }

  /**
   * Gestione dell'accoppiamento
   */
  while(TRUE) {
    informazioni_accoppiamento informazioni_a;
    informazioni_accoppiamento informazioni_b;
    msg_ricevi_messaggio_notifica_accoppiamento(msg_gestore_a, &informazioni_a);
    msg_ricevi_messaggio_notifica_accoppiamento(msg_gestore_b, &informazioni_b);

    if (informazioni_a.pid_mittente == informazioni_b.pid_coniuge) {
      // Recupera individui e rimuovi i genitori dalla shm
      caratteristiche_individuo individuo_a = recupera(informazioni_a.pid_mittente, init_people, descrizione, sem_shm_descrizione_id, 'A');
      caratteristiche_individuo individuo_b = recupera(informazioni_b.pid_mittente, init_people, descrizione, sem_shm_descrizione_id, 'B');
      char tipo_scelto = 'C';
      // Crea nuovi individui
      caratteristiche_individuo nuovo_individuo_1;
      crea_individuo_da_coppia(&nuovo_individuo_1, individuo_a, individuo_b, genes, 0, individui_A, individui_B, init_people, &tipo_scelto);
      caratteristiche_individuo nuovo_individuo_2;
      crea_individuo_da_coppia(&nuovo_individuo_2, individuo_a, individuo_b, genes, 1, individui_A, individui_B, init_people, &tipo_scelto);
      
      // Aggiunge individui nella memoria condivisa
      if ((nuovo_individuo_1.tipo == 'A' && nuovo_individuo_2.tipo == 'B') || (nuovo_individuo_1.tipo == 'B' && nuovo_individuo_2.tipo == 'A')) {
        avvia_individuo(nuovo_individuo_1, init_people, 2);
        avvia_individuo(nuovo_individuo_2, init_people, 2);
        sem_rilascia(sem_azione_id);
      } else if (nuovo_individuo_1.tipo == 'A' && nuovo_individuo_2.tipo == 'A') {
        avvia_individuo(nuovo_individuo_1, init_people, 2);
        sem_riserva(sem_shm_a_id);
        avvia_individuo(nuovo_individuo_2, init_people, 1);
        sem_rilascia(sem_azione_b_id);
        sem_rilascia(sem_shm_b_id);
      } else {
        avvia_individuo(nuovo_individuo_1, init_people, 2);
        sem_riserva(sem_shm_b_id);
        avvia_individuo(nuovo_individuo_2, init_people, 1);
        sem_rilascia(sem_azione_a_id);
        sem_rilascia(sem_shm_a_id);
      }
    } else {
      printf("Errore durante il recupero delle informazioni sull'accoppiamento.\n");
      exit(EXIT_FAILURE);
    }
  }
}
