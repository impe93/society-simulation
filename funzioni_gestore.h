#ifndef __FUNZIONI_GESTORE__

#define __FUNZIONI_GESTORE__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#include "tipi_simulatore_societa.h"
#include "gestione_semafori.h"
#include "gestione_shm.h"

/**
 * Controlla che la stringa passata come parametro rappresenti un intero non segnato
 * 
 * @param {char*} stringa: La stringa da controllare
 * @return {bool}: TRUE se la stringa è un numero non segnato, FALSE altrimenti.
 */
bool isUnsignedNumber(char* stringa);

/**
 * Avvia un nuovo individuo con le caratteristiche specificate nel parametro individuo
 * 
 * @param {caratteristiche_individuo} individuo: la struttura che ha specificate le caratteristiche
 * dell'individuo
 */
void avvia_individuo (caratteristiche_individuo individuo, int init_people, int inizializzazione);

/**
 * Crea casualmente le proprietà che dovrà avere il nuovo individuo e lo avvia.
 * 
 * @param {unsigned long} genes: Rappresenta fino a che numero un gene può arrivare, serve per
 * limitare il valore casuale
 * @param {int} init_people: 
 */
caratteristiche_individuo crea_individuo (unsigned long genes, int init_people);

/**
 * Inizializza tanti individui in base a quanto è segnato nel parametro init_people
 * 
 * @param {int} init_people: Il numero di individui da inizializzare
 * @param {unsigned long} genes: Il massimo valore che può assumere il genoma di un individuo.
 * @param {pid_t []} individui_da_avviare: Ad ogni creazione di un individuo il suo pid viene messo
 * all'interno di questo array.
 */
void inizializza_individui(int init_people, unsigned long genes);

/**
 * Inizializza l'array di rappresentazioni degli individui di un detrminato tipo
 * con valori di default
 * 
 * @param {int} shm_id: L'ID della shared memory di individui da inizializzare
 * @param {int} init_people: Il numero di individui totali che saranno presenti
 * all'interno del programma
 */
void inizializza_shm(int shm_id, int init_people);

/**
 * Manda un segnale di terminazione a tutti i processi attivi inerenti alla simulazione
 * 
 * @param {int} sim_time: Il tempo da attendere prima dell'invio del segnale
 * @param {int} init_people: la massima quantità di elementi che può essere presente
 * all'interno dell'array
 * @param {int} pid_gestore: Il pid del processo gestore
 * @param {int} pid_terminatore: Il pid del processo terminatore
 * 
 * @error: Conclude il processo.
 */
void terminazione_simulazione(int sim_time, int init_people, pid_t pid_gestore, pid_t pid_terminatore_processi);

/**
 * Sceglie un tipo casuale di processo tra A e B e ne ritorna il risultato.
 * 
 * @return {char}: Il tipo di processo scelto
 */
char scelta_tipo_processo();

/**
 * Invia il segnale passato come parametro al pid passato come parametro e ne gestisce l'errore
 * 
 * @param: Il pid a cui si vuole inviare il segnale
 * @param: Il segnale che si vuole inviare al pid
 * 
 * @error: Termina il processo chiamante
 */
void invio_segnale(pid_t pid, int segnale);

/**
 * Conta gli individui presenti in un array di rappresentazione_individuo passato come
 * parametro che ha al massimo init_people - 1 elementi.
 * 
 * @param {int} individui: L'array con tutte le rappresentazioni degli individui al suo interno
 * @param {int} init_people: sottraendo 1 a questo parametro si ottiene la grandezza massima
 * dell'array
 */
int conta_individui_attivi(rappresentazione_individuo individui [], int init_people);

/**
 * Genera e ritorna un numero random tra il minimo ed il massimo che sono passati come parametro
 * 
 * @param {unsigned int} min: Il minimo numero che deve essere generato
 * @param {unsigned int} max: Il massimo numero che deve essere generato
 * @return {int}: Un numero casualmente generato
 */
int numero_random(unsigned int min, unsigned int max);

/**
 * Cerca l'individuo da terminare e gli invia un segnale di terminazione e dopo attende
 * la sua terinazione.
 * 
 * @param {rappresentazione_individuo []} individui: L'array di individui su cui si
 * trova quello da terminare
 * @param {int}: il numero dell'individuo, tra gli individui utilizzati, scelto per
 * essere terminato
 */
void termina_individuo(rappresentazione_individuo individui [], int numero_da_terminare, descrizione_simulazione* descrizione);

/**
 * Si occupa di terminare un processo ogni birth_death secondi, scegliendolo casualmente
 * tra l'array di A e quello di B
 * 
 * @param {int} init_people: La lunghezza massima che possono avere gli array di elementi
 * su cui cercare il processo da terminare
 * @param {int} birth_death: L'intervallo di tempo in secondi che deve passare tra una
 * terminazione e l'altra
 * @param {descrizione_individui} descrizione: Una struttura che deve essere aggiornata
 * ad ogni terminazione e ad ogni creazione di un nuovo individuo.
 */
void attivita_terminatore_individui(int init_people, int birth_death, unsigned long genes, descrizione_simulazione* descrizione_simulazione);

/**
 * Chiamato quando un processo casuale di tipo A deve terminare, il metodo lo sceglie,
 * lo termina ed aggiorna la descrizione.
 * 
 * @param {int} init_people: La lunghezza massima che può avere l'array di individui
 * @param {descrizione_simulazione*} descrizione: La struttura che descrive la simulazione
 */
void preparazione_terminazione_a(int init_people, descrizione_simulazione* descrizione);

/**
 * Chiamato quando un processo casuale di tipo B deve terminare, il metodo lo sceglie,
 * lo termina ed aggiorna la descrizione.
 * 
 * @param {int} init_people: La lunghezza massima che può avere l'array di individui
 * @param {descrizione_simulazione*} descrizione: La struttura che descrive la simulazione
 */
void preparazione_terminazione_b(int init_people, descrizione_simulazione* descrizione);

/**
 * Misura la lunghezza della stringa passata come paramentro
 * 
 * @param {char*} stringa: La stringa di cui si vuole misurare la lunghezza
 * @return {int}: la lunghezza della stringa passata come parametro
 */
int lunghezza_stringa(char* stringa);

/**
 * Compara i due nomi passati come parametro
 * 
 * @param {char*} nome1: Il primo nome che si vuole comparare
 * @param {char*} nome2: Il secondo nome che si vuole comparare
 * @return: True se nome1 è più lungo di nome2, false altrimenti
 */
bool nome_piu_lungo(char* nome1, char* nome2);

/**
 * Aggiorna la descrizione della simulazione con i dati del nuovo individuo che è
 * passato come parametro
 * 
 * @param {caratteristiche_individuo} individuo: L'individuo con cui si vuole
 * aggiornare la descrizione della simulazione
 */
void aggiorna_descrizione_nuovo_individuo(caratteristiche_individuo individuo);

/**
 * Stampa su terminale lo stato attuale della simulazione
 */
void aggiorna_utente_stato_simulazione();

/**
 * Recupera l'individuo con pid uguale al parametro "pid_a" dalla shm A
 * 
 * @param {pid_t} pid_a: Il pid di un individuo di tipo A che si trova sulla shm A
 * @param {int} init_people: Il numero totale di individui
 * @return {caratteristiche_individuo}: Le caratteristiche dell'individuo trovato
 * nella shm con PID uguale al valore passato come parametro. Individuo vuoto se
 * non è stato trovato l'individuo con il PID desiderato
 */
caratteristiche_individuo recupera_a(pid_t pid_a, int init_people, descrizione_simulazione* descrizione, int sem_shm_descrizione_id);

/**
 * Recupera l'individuo con pid uguale al parametro "pid_b" dalla shm B
 * 
 * @param {pid_t} pid_b: Il pid di un individuo di tipo A che si trova sulla shm A
 * @param {int} init_people: Il numero totale di individui
 * @return {caratteristiche_individuo}: Le caratteristiche dell'individuo trovato
 * nella shm con PID uguale al valore passato come parametro. Individuo vuoto se
 * non è stato trovato l'individuo con il PID desiderato.
 */
caratteristiche_individuo recupera_b(pid_t pid_b, int init_people, descrizione_simulazione* descrizione, int sem_shm_descrizione_id);

// Definizione metodi

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

caratteristiche_individuo crea_individuo (unsigned long genes, int init_people) {
    caratteristiche_individuo individuo;

    int shm_a_id = shm_recupero(SHM_A_KEY, init_people - 1);
    int shm_b_id = shm_recupero(SHM_B_KEY, init_people - 1);
    rappresentazione_individuo* individui_A;
    rappresentazione_individuo* individui_B;
    shm_attach_rappresentazione_individuo(shm_a_id, &individui_A);
    shm_attach_rappresentazione_individuo(shm_b_id, &individui_B);
    int numero_A = conta_individui_attivi(individui_A, init_people);
    int numero_B = conta_individui_attivi(individui_B, init_people);

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
    individuo.genoma = (rand() % (genes + 1)) + 2;
    strcpy(individuo.nome, "");
    individuo.nome[0] = (char)(rand() % 25) + 65;
    individuo.nome[1] = '\0';

    shm_detach_rappresentazione_individuo(individui_A);
    shm_detach_rappresentazione_individuo(individui_B);

    return individuo;
}

caratteristiche_individuo crea_individuo_init (unsigned long genes) {
    caratteristiche_individuo individuo;

    if (rand() % 2) {
        individuo.tipo = 'A';
    } else {
        individuo.tipo = 'B';
    }
    individuo.genoma = (rand() % (genes + 1)) + 2;
    strcpy(individuo.nome, "");
    individuo.nome[0] = (char)(rand() % 25) + 65;
    individuo.nome[1] = '\0';

    return individuo;
}

caratteristiche_individuo crea_A (unsigned long genes) {
    caratteristiche_individuo individuo;

    individuo.tipo = 'A';
    individuo.genoma = (rand() % (genes + 1)) + 2;
    strcpy(individuo.nome, "");
    individuo.nome[0] = (char)(rand() % 25) + 65;
    individuo.nome[1] = '\0';

    return individuo;
}

caratteristiche_individuo crea_B (unsigned long genes) {
    caratteristiche_individuo individuo;

    individuo.tipo = 'B';
    individuo.genoma = (rand() % (genes + 1)) + 2;
    strcpy(individuo.nome, "");
    individuo.nome[0] = (char)(rand() % 25) + 65;
    individuo.nome[1] = '\0';

    return individuo;
}

void inizializza_individui(int init_people, unsigned long genes) {
    int sem_shm_a_id = sem_recupero(SEM_SHM_A);
    int sem_shm_b_id = sem_recupero(SEM_SHM_B);
    caratteristiche_individuo individuo_A = crea_A(genes);
    caratteristiche_individuo individuo_B = crea_B(genes);
    sem_riserva(sem_shm_b_id);
    avvia_individuo(individuo_B, init_people, 0);
    sem_riserva(sem_shm_a_id);
    avvia_individuo(individuo_A, init_people, 0);
    for(int i = 0; i < init_people - 2; i++) {
        caratteristiche_individuo individuo = crea_individuo_init(genes);
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

    int sem_shm_a_id = sem_recupero(SEM_SHM_A);
    int sem_shm_b_id = sem_recupero(SEM_SHM_B);
    int sem_azione_id = sem_recupero(SEM_AZIONE);
    int sem_azione_a_id = sem_recupero(SEM_AZIONE_A);
    int sem_azione_b_id = sem_recupero(SEM_AZIONE_B);

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

    for(int i = 0; i < init_people - 1; i++) {
        sem_riserva(sem_shm_a_id);
        sem_riserva(sem_shm_b_id);
        if (shm_a[i].utilizzata) {
            invio_segnale(shm_a[i].pid, SIGTERM);
        } else {
            sem_rilascia(sem_shm_a_id);
        }
        if (shm_b[i].utilizzata) {
            invio_segnale(shm_b[i].pid, SIGTERM);
        } else {
            sem_rilascia(sem_shm_b_id);
        }
    }

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

void preparazione_terminazione_a(int init_people, descrizione_simulazione* descrizione) {
    int sem_shm_a_id = sem_recupero(SEM_SHM_A);
    sem_riserva(sem_shm_a_id);
    int shm_a_id = shm_recupero(SHM_A_KEY, init_people - 1);
    rappresentazione_individuo* individui;
    shm_attach_rappresentazione_individuo(shm_a_id, &individui);
    int individui_attivi = conta_individui_attivi(individui, init_people);
    int numero_da_terminare = numero_random(1, individui_attivi);
    termina_individuo(individui, numero_da_terminare, descrizione);
    shm_detach_rappresentazione_individuo(individui);
}

void preparazione_terminazione_b(int init_people, descrizione_simulazione* descrizione) {
    int sem_shm_b_id = sem_recupero(SEM_SHM_B);
    sem_riserva(sem_shm_b_id);
    int shm_b_id = shm_recupero(SHM_B_KEY, init_people - 1);
    rappresentazione_individuo* individui;
    shm_attach_rappresentazione_individuo(shm_b_id, &individui);
    int individui_attivi = conta_individui_attivi(individui, init_people);
    int numero_da_terminare = numero_random(1, individui_attivi);
    termina_individuo(individui, numero_da_terminare, descrizione);
    shm_detach_rappresentazione_individuo(individui);
}

int lunghezza_stringa(char* stringa) {
    int lunghezza = 0;
    for(int i = 0; *(stringa + i) != '\0'; i++) {
        lunghezza++;
    }
    return lunghezza;
}

bool nome_piu_lungo(char* nome1, char* nome2) {
    if (lunghezza_stringa(nome1) > lunghezza_stringa(nome2)) {
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
            preparazione_terminazione_a(init_people, descrizione);
        } else {
            preparazione_terminazione_b(init_people, descrizione);
        }
        sem_riserva(sem_shm_a_id);
        sem_riserva(sem_shm_b_id);
        caratteristiche_individuo nuovo_individuo = crea_individuo(genes, init_people);
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

caratteristiche_individuo recupera_a(pid_t pid_a, int init_people, descrizione_simulazione* descrizione, int sem_shm_descrizione_id) {
    caratteristiche_individuo individuo;
    individuo.tipo = 'C';
    rappresentazione_individuo* individui; 
    int shm_a_id = shm_recupero(SHM_A_KEY, init_people - 1);
    shm_attach_rappresentazione_individuo(shm_a_id, &individui);
    for (int i = 0; i < init_people - 1; i++) {
        if (individui[i].utilizzata && individui[i].pid == pid_a) {
            individuo = individui[i].caratteristiche;
            individui[i].utilizzata = FALSE;
            sem_riserva(sem_shm_descrizione_id);
            (*descrizione).processi_totali_terminati++;
            ((*descrizione).individui_a_attivi)--;
            sem_rilascia(sem_shm_descrizione_id);
        }
    }
    shm_detach_rappresentazione_individuo(individui);
    return individuo;
}

caratteristiche_individuo recupera_b(pid_t pid_b, int init_people, descrizione_simulazione* descrizione, int sem_shm_descrizione_id) {
    caratteristiche_individuo individuo;
    individuo.tipo = 'C';
    rappresentazione_individuo* individui; 
    int shm_b_id = shm_recupero(SHM_B_KEY, init_people - 1);
    shm_attach_rappresentazione_individuo(shm_b_id, &individui);
    for (int i = 0; i < init_people - 1; i++) {
        if (individui[i].utilizzata && individui[i].pid == pid_b) {
            individuo = individui[i].caratteristiche;
            individui[i].utilizzata = FALSE;
            sem_riserva(sem_shm_descrizione_id);
            (*descrizione).processi_totali_terminati++;
            ((*descrizione).individui_b_attivi)--;
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

void crea_individuo_da_coppia(caratteristiche_individuo* nuovo_individuo, caratteristiche_individuo individuo_a,caratteristiche_individuo individuo_b, unsigned long genes, bool scelta_nome, rappresentazione_individuo* individui_A, rappresentazione_individuo* individui_B, int init_people, char* tipo_scelto) {
    // Concateno i nomi dei due individui e aggiungo una lettera casuale al fondo
    // Potrebbe causare segmentation fault
    if (scelta_nome) {
        strncpy(nuovo_individuo->nome, individuo_a.nome, sizeof(char) * (LUNGHEZZA_NOME - 1));
    } else {
        strncpy(nuovo_individuo->nome, individuo_b.nome, sizeof(char) * (LUNGHEZZA_NOME - 1));
    }
    char char_string[2];
    char_string[0] = (char)(rand() % 25) + 65;
    char_string[1] = '\0';
    strncat(nuovo_individuo->nome, char_string, LUNGHEZZA_NOME - strlen(nuovo_individuo->nome) - 1);

    // Calcolo il Genoma del nuovo individuo
    unsigned long mcd_a_b = 0;
    if (individuo_b.genoma % individuo_a.genoma == 0) {
        mcd_a_b = individuo_a.genoma;
    } else {
        if (individuo_a.genoma >= individuo_b.genoma) {
            mcd_a_b = mcd(individuo_a.genoma, individuo_b.genoma);
        } else {
            mcd_a_b = mcd(individuo_b.genoma, individuo_a.genoma);
        }
    }

    nuovo_individuo->genoma = (rand() % genes) + mcd_a_b;

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

#endif
