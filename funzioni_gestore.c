#include "funzioni_gestore.h"

bool isUnsignedNumber(char* stringa) {
  bool risultato = TRUE;
  for(int i = 0; *(stringa + i) != '\0' && risultato; i++) {
    if (!isdigit(*(stringa + i))) {
      risultato = FALSE;
    }
  }
  
  return risultato;
}

void avvia_individuo (caratteristiche_individuo individuo, int init_people, int inizializzazione) {
  pid_t pid_figlio = 0;
  // Sleep 50 ms
  usleep(50 * 1000);
  switch(pid_figlio = fork()) {
    case -1: {
      printf("Errore durante la creazione di un nuovo figlio.\n");
      exit(EXIT_FAILURE);
    }
    case 0: {
      char stringa_genoma [128];
      char stringa_init_people [128];
      char inizializzazione_stringa [10];
      sprintf(stringa_genoma, "%lu", individuo.genoma);
      sprintf(stringa_init_people, "%i", init_people);
      sprintf(inizializzazione_stringa, "%i", inizializzazione);
      if (individuo.tipo == 'A') {
        if (execl("./tipo_A", &individuo.tipo, individuo.nome, stringa_genoma, stringa_init_people, inizializzazione_stringa, NULL) == -1) {
          printf("Errore durante la creazione del nuovo individuo A.\n");
          exit(EXIT_FAILURE);
        }
      } else if (individuo.tipo == 'B') {
        if (execl("./tipo_B", &individuo.tipo, individuo.nome, stringa_genoma, stringa_init_people, inizializzazione_stringa, NULL) == -1) {
          printf("Errore durante la creazione del nuovo individuo B.\n");
          exit(EXIT_FAILURE);
        }
      }
    }
    default: {
      aggiorna_descrizione_nuovo_individuo(individuo);
    }
  }
}

caratteristiche_individuo crea_individuo (unsigned long genes, int init_people, char tipo) {
  caratteristiche_individuo individuo;

  int shm_a_id = shm_recupero(SHM_A_KEY, init_people - 1);
  int shm_b_id = shm_recupero(SHM_B_KEY, init_people - 1);
  rappresentazione_individuo* individui_A;
  rappresentazione_individuo* individui_B;
  shm_attach_rappresentazione_individuo(shm_a_id, &individui_A);
  shm_attach_rappresentazione_individuo(shm_b_id, &individui_B);
  int numero_A = conta_individui_attivi(individui_A, init_people);
  int numero_B = conta_individui_attivi(individui_B, init_people);
  
  if (tipo == 'A') {
    individuo.tipo = 'A';
  } else if (tipo == 'B') {
    individuo.tipo = 'B';
  } else {
    if (rand() % 2) {
      if (numero_B > 0) {
        individuo.tipo = 'A';
      } else {
        individuo.tipo = 'B';
      }
    } else {
      if (numero_A > 0) {
        individuo.tipo = 'B';
      } else {
        individuo.tipo = 'A';
      }
    }
  }
  individuo.genoma = (rand() % (genes + 1)) + 2;
  strcpy(individuo.nome, "");
  individuo.nome[0] = (char)(rand() % 25) + 65;
  individuo.nome[1] = '\0';

  shm_detach_rappresentazione_individuo(individui_A);
  shm_detach_rappresentazione_individuo(individui_B);

  return individuo;
}

void inizializza_individui(int init_people, unsigned long genes) {
  int sem_shm_a_id = sem_recupero(SEM_SHM_A);
  int sem_shm_b_id = sem_recupero(SEM_SHM_B);
  caratteristiche_individuo individuo_A = crea_individuo(genes, init_people, 'A');
  caratteristiche_individuo individuo_B = crea_individuo(genes, init_people, 'B');
  sem_riserva(sem_shm_b_id);
  avvia_individuo(individuo_B, init_people, 0);
  sem_riserva(sem_shm_a_id);
  avvia_individuo(individuo_A, init_people, 0);
  for(int i = 0; i < init_people - 2; i++) {
    caratteristiche_individuo individuo = crea_individuo(genes, init_people, 'C');
    if (individuo.tipo == 'A') {
      sem_riserva(sem_shm_a_id);
    } else {
      sem_riserva(sem_shm_b_id);
    }
    avvia_individuo(individuo, init_people, 0);
  }
}

void inizializza_shm(int shm_id, int init_people) {
  rappresentazione_individuo* individui;
  shm_attach_rappresentazione_individuo(shm_id, &individui);
  for (int i = 0; i < init_people - 1; i++) {
    individui[i].utilizzata = FALSE;
    individui[i].pid = 0;
    individui[i].caratteristiche.tipo = 'C';
    individui[i].caratteristiche.genoma = 0;
    strcpy(individui[i].caratteristiche.nome, "");
  }
  shm_detach_rappresentazione_individuo(individui);
}

void invio_segnale(pid_t pid, int segnale) {
  if (kill(pid, segnale) == -1) {
    printf("Errore durante l'invio del segnale di terminazione al processo A.\n");
    exit(EXIT_FAILURE);
  }
}

void terminazione_simulazione(int sim_time, int init_people, pid_t pid_gestore, pid_t pid_terminatore_processi) {
  sleep(sim_time);

  rappresentazione_individuo* shm_a;
  rappresentazione_individuo* shm_b;
  descrizione_simulazione* descrizione;

  int sem_shm_descrizione_id = sem_recupero(SEM_SHM_DESCRIZIONE);
  int shm_descrizione_id = shm_recupero_descrizione(SHM_DESCRIZIONE_KEY);
  int sem_shm_a_id = sem_recupero(SEM_SHM_A);
  int sem_shm_b_id = sem_recupero(SEM_SHM_B);
  int sem_azione_id = sem_recupero(SEM_AZIONE);
  int sem_azione_a_id = sem_recupero(SEM_AZIONE_A);
  int sem_azione_b_id = sem_recupero(SEM_AZIONE_B);

  shm_attach_descrizione_simulazione(shm_descrizione_id, &descrizione);

  shm_attach_rappresentazione_individuo(shm_recupero(SHM_A_KEY, init_people - 1), &shm_a);
  shm_attach_rappresentazione_individuo(shm_recupero(SHM_B_KEY, init_people - 1), &shm_b);
  
  sem_riserva(sem_azione_id);
  sem_riserva(sem_azione_a_id);
  sem_riserva(sem_azione_b_id);
  
  sem_riserva(sem_shm_a_id);
  sem_riserva(sem_shm_b_id);
  invio_segnale(pid_gestore, SIGTERM);
  invio_segnale(pid_terminatore_processi, SIGTERM);
  sem_rilascia(sem_shm_b_id);
  sem_rilascia(sem_shm_a_id);

  sem_riserva(sem_shm_descrizione_id);
  for(int i = 0; i < init_people - 1; i++) {
    sem_riserva(sem_shm_a_id);
    sem_riserva(sem_shm_b_id);
    if (shm_a[i].utilizzata) {
      invio_segnale(shm_a[i].pid, SIGTERM);
      (*descrizione).processi_totali_terminati++;
      (*descrizione).individui_a_attivi--;
    } else {
      sem_rilascia(sem_shm_a_id);
    }
    if (shm_b[i].utilizzata) {
      invio_segnale(shm_b[i].pid, SIGTERM);
      (*descrizione).processi_totali_terminati++;
      (*descrizione).individui_b_attivi--;
    } else {
      sem_rilascia(sem_shm_b_id);
    }
  }
  sem_rilascia(sem_shm_descrizione_id);

  shm_detach_rappresentazione_individuo(shm_a);
  shm_detach_rappresentazione_individuo(shm_b);
}

char scelta_tipo_processo() {
  char tipo_da_terminare = 'A';
  if (rand() % 2) {
    tipo_da_terminare = 'B';
  }
  return tipo_da_terminare;
}

int conta_individui_attivi(rappresentazione_individuo individui [], int init_people) {
  int conto_individui = 0;
  for (int i = 0; i < init_people - 1; i++) {
    if (individui[i].utilizzata) {
      conto_individui++;
    }
  }
  return conto_individui;
}

int numero_random(unsigned int min, unsigned int max) {
  if (min >= max) {
      return min;
  }
  return rand() % (max - min) + min;
}

void termina_individuo(rappresentazione_individuo individui [], int numero_da_terminare, descrizione_simulazione* descrizione) {
  int sem_shm_descrizione_id = sem_recupero(SEM_SHM_DESCRIZIONE);
  int i = 0;
  int j = 0;
  for(j = 0; i < numero_da_terminare; j++) {
    if (individui[j].utilizzata == TRUE) {
      i++;
      if(i == numero_da_terminare) {
        break;
      }
    }
  }
  rappresentazione_individuo individuo_da_terminare = individui[j];
  invio_segnale(individuo_da_terminare.pid, SIGTERM);
  sem_riserva(sem_shm_descrizione_id);
  (*descrizione).processi_totali_terminati++;
  if (individuo_da_terminare.caratteristiche.tipo == 'A') {
    ((*descrizione).individui_a_attivi)--;
  } else {
    ((*descrizione).individui_b_attivi)--;
  }
  sem_rilascia(sem_shm_descrizione_id);
}

void preparazione_terminazione(int init_people, descrizione_simulazione* descrizione, char tipo_terminazione) {
  int sem_shm_id = 0;
  int shm_id = 0;
  if (tipo_terminazione == 'A') {
    sem_shm_id = sem_recupero(SEM_SHM_A);
    shm_id = shm_recupero(SHM_A_KEY, init_people - 1);
  } else {
    sem_shm_id = sem_recupero(SEM_SHM_B);
    shm_id = shm_recupero(SHM_B_KEY, init_people - 1);
  }
  sem_riserva(sem_shm_id);
  rappresentazione_individuo* individui;
  shm_attach_rappresentazione_individuo(shm_id, &individui);
  int individui_attivi = conta_individui_attivi(individui, init_people);
  int numero_da_terminare = numero_random(1, individui_attivi);
  termina_individuo(individui, numero_da_terminare, descrizione);
  shm_detach_rappresentazione_individuo(individui);
}

bool nome_piu_lungo(char* nome1, char* nome2) {
  if (strlen(nome1) > strlen(nome2)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

void aggiorna_descrizione_nuovo_individuo(caratteristiche_individuo individuo) {
  int sem_shm_descrizione_id = sem_recupero(SEM_SHM_DESCRIZIONE);
  int shm_descrizione_id = shm_recupero_descrizione(SHM_DESCRIZIONE_KEY);
  descrizione_simulazione* descrizione = NULL;
  shm_attach_descrizione_simulazione(shm_descrizione_id, &descrizione);
  sem_riserva(sem_shm_descrizione_id);
  if (individuo.genoma > (*descrizione).individuo_genoma_lungo.genoma) {
    (*descrizione).individuo_genoma_lungo = individuo;
  }
  if (nome_piu_lungo(individuo.nome, (*descrizione).individuo_nome_lungo.nome)) {
    (*descrizione).individuo_nome_lungo = individuo;
  }
  if (individuo.tipo == 'A') {
    ((*descrizione).individui_a_attivi)++;
    ((*descrizione).individui_a_creati)++;
  } else {
    ((*descrizione).individui_b_attivi)++;
    ((*descrizione).individui_b_creati)++;
  }
  sem_rilascia(sem_shm_descrizione_id);
  shm_detach_descrizione_simulazione(descrizione);
}

void attivita_terminatore_individui(int init_people, int birth_death, unsigned long genes, descrizione_simulazione* descrizione) {
  int sem_shm_a_id = sem_recupero(SEM_SHM_A);
  int sem_shm_b_id = sem_recupero(SEM_SHM_B);
  for(;;) {
    sleep(birth_death);
    char tipo_processo_scelto = scelta_tipo_processo();
    sem_riserva(sem_recupero(SEM_AZIONE));
    sem_riserva(sem_recupero(SEM_AZIONE_A));
    sem_riserva(sem_recupero(SEM_AZIONE_B));
    if (tipo_processo_scelto == 'A') {
      preparazione_terminazione(init_people, descrizione, 'A');
    } else {
      preparazione_terminazione(init_people, descrizione, 'B');
    }
    sem_riserva(sem_shm_a_id);
    sem_riserva(sem_shm_b_id);
    caratteristiche_individuo nuovo_individuo = crea_individuo(genes, init_people, 'C');
    if (nuovo_individuo.tipo == 'A') {
      sem_rilascia(sem_recupero(SEM_AZIONE_B));
      sem_rilascia(sem_shm_b_id);
      avvia_individuo(nuovo_individuo, init_people, 2);
    } else {
      sem_rilascia(sem_shm_a_id);
      sem_rilascia(sem_recupero(SEM_AZIONE_A));
      avvia_individuo(nuovo_individuo, init_people, 2);
    }
    sem_rilascia(sem_recupero(SEM_AZIONE));
    aggiorna_utente_stato_simulazione();
  }
}

void aggiorna_utente_stato_simulazione() {
  int shm_descrizione_id = shm_recupero_descrizione(SHM_DESCRIZIONE_KEY);
  int sem_shm_descrizione_id = sem_recupero(SEM_SHM_DESCRIZIONE);
  descrizione_simulazione* descrizione = NULL;
  shm_attach_descrizione_simulazione(shm_descrizione_id, &descrizione);
  sem_riserva(sem_shm_descrizione_id);
  printf("\nAggiornamento:\n");
  printf("Individui di tipo A attivi: %i\n", (*descrizione).individui_a_attivi);
  printf("Individui di tipo B attivi: %i\n", (*descrizione).individui_b_attivi);
  printf("Individui di tipo A creati: %i\n", (*descrizione).individui_a_creati);
  printf("Individui di tipo B creati: %i\n", (*descrizione).individui_b_creati);
  printf("Individui totali terminati: %i\n", (*descrizione).processi_totali_terminati);
  sem_rilascia(sem_shm_descrizione_id);
  shm_detach_descrizione_simulazione(descrizione);
}

void aggiorna_utente_terminazione_simulazione() {
  int shm_descrizione_id = shm_recupero_descrizione(SHM_DESCRIZIONE_KEY);
  int sem_shm_descrizione_id = sem_recupero(SEM_SHM_DESCRIZIONE);
  descrizione_simulazione* descrizione = NULL;
  shm_attach_descrizione_simulazione(shm_descrizione_id, &descrizione);
  sem_riserva(sem_shm_descrizione_id);
  printf("\nAGGIORNAMENTO DI TERMINAZIONE:\n");
  printf("Individui di tipo A creati in totale: %i\n", (*descrizione).individui_a_creati);
  printf("Individui di tipo B creati in totale: %i\n", (*descrizione).individui_b_creati);
  printf("Individui terminati in totale: %i\n", (*descrizione).processi_totali_terminati);
  printf("Genoma più alto raggiunto: %lu\n",(*descrizione).individuo_genoma_lungo.genoma);
  printf("Nome più lungo raggiunto: %s\n",(*descrizione).individuo_nome_lungo.nome);
  sem_rilascia(sem_shm_descrizione_id);
  shm_detach_descrizione_simulazione(descrizione);
}

caratteristiche_individuo recupera(pid_t pid, int init_people, descrizione_simulazione* descrizione, int sem_shm_descrizione_id, char tipo) {
  caratteristiche_individuo individuo;
  individuo.tipo = 'C';
  rappresentazione_individuo* individui;
  int shm_id = 0;
  if (tipo == 'A') {
    shm_id = shm_recupero(SHM_A_KEY, init_people - 1);
  } else {
    shm_id = shm_recupero(SHM_B_KEY, init_people - 1);
  }
  shm_attach_rappresentazione_individuo(shm_id, &individui);
  for (int i = 0; i < init_people - 1; i++) {
    if (individui[i].utilizzata && individui[i].pid == pid) {
      individuo = individui[i].caratteristiche;
      individui[i].utilizzata = FALSE;
      sem_riserva(sem_shm_descrizione_id);
      (*descrizione).processi_totali_terminati++;
      if (tipo == 'A') {
        ((*descrizione).individui_a_attivi)--;
      } else {
        ((*descrizione).individui_b_attivi)--;
      }
      sem_rilascia(sem_shm_descrizione_id);
    }
  }
  shm_detach_rappresentazione_individuo(individui);
  return individuo;
}

unsigned long mcd(unsigned long a, unsigned long b) {
  int r = 0;
  while(b != 0) {
    r = a % b;
    a = b; 
    b = r;
  }
  return a;
}

unsigned long genoma_nuovo_da_coppia(unsigned long genoma_individuo_a, unsigned long genoma_individuo_b, unsigned long genes) {
  unsigned long mcd_a_b = 0;
  if (genoma_individuo_b % genoma_individuo_a == 0) {
    mcd_a_b = genoma_individuo_a;
  } else {
    if (genoma_individuo_a >= genoma_individuo_b) {
      mcd_a_b = mcd(genoma_individuo_a, genoma_individuo_b);
    } else {
      mcd_a_b = mcd(genoma_individuo_b, genoma_individuo_a);
    }
  }

  return (rand() % genes) + mcd_a_b;
}

void nome_nuovo_da_coppia(bool scelta_nome, char* nome_nuovo_individuo, char* nome_a, char* nome_b) {
  if (scelta_nome) {
    strncpy(nome_nuovo_individuo, nome_a, sizeof(char) * (LUNGHEZZA_NOME - 1));
  } else {
    strncpy(nome_nuovo_individuo, nome_b, sizeof(char) * (LUNGHEZZA_NOME - 1));
  }
  char char_string[2];
  char_string[0] = (char)(rand() % 25) + 65;
  char_string[1] = '\0';
  strncat(nome_nuovo_individuo, char_string, LUNGHEZZA_NOME - strlen(nome_nuovo_individuo) - 1);
}

void crea_individuo_da_coppia(caratteristiche_individuo* nuovo_individuo, caratteristiche_individuo individuo_a,caratteristiche_individuo individuo_b, unsigned long genes, bool scelta_nome, rappresentazione_individuo* individui_A, rappresentazione_individuo* individui_B, int init_people, char* tipo_scelto) {
  nome_nuovo_da_coppia(scelta_nome, nuovo_individuo->nome, individuo_a.nome, individuo_b.nome);
  nuovo_individuo->genoma = genoma_nuovo_da_coppia(individuo_a.genoma, individuo_b.genoma, genes);

  // Calcolo tipo dell'individuo
  if (rand() % 2) {
    if (*tipo_scelto == 'C') {
      if (conta_individui_attivi(individui_B, init_people) < 1) {
        nuovo_individuo->tipo = 'B';
        *tipo_scelto = 'B';
      } else {
        nuovo_individuo->tipo = 'A';
        *tipo_scelto = 'A';
      }
    } else if (conta_individui_attivi(individui_B, init_people) == 0 && conta_individui_attivi(individui_A, init_people) == 0 && *tipo_scelto == 'B') {
      nuovo_individuo->tipo = 'A';
    } else if (conta_individui_attivi(individui_B, init_people) == 0 && conta_individui_attivi(individui_A, init_people) == 0 && *tipo_scelto == 'A'){
      nuovo_individuo->tipo = 'B';
    } else if (conta_individui_attivi(individui_B, init_people) < 1) {
      nuovo_individuo->tipo = 'B';
    } else {
      nuovo_individuo->tipo = 'A';
    }
  } else {
    if (*tipo_scelto == 'C') {
      if (conta_individui_attivi(individui_A, init_people) < 1) {
        nuovo_individuo->tipo = 'A';
        *tipo_scelto = 'A';
      } else {
        nuovo_individuo->tipo = 'B';
        *tipo_scelto = 'B';
      }
    } else if (conta_individui_attivi(individui_B, init_people) == 0 && conta_individui_attivi(individui_A, init_people) == 0 && *tipo_scelto == 'B') {
      nuovo_individuo->tipo = 'A';
    } else if (conta_individui_attivi(individui_B, init_people) == 0 && conta_individui_attivi(individui_A, init_people) == 0 && *tipo_scelto == 'A'){
      nuovo_individuo->tipo = 'B';
    } else if (conta_individui_attivi(individui_A, init_people) < 1) {
      nuovo_individuo->tipo = 'A';
    } else {
      nuovo_individuo->tipo = 'B';
    }
  }
}