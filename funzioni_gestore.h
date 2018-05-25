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
 * @param {int} init_peolple: il numero massimo d'individui all'interno della simulazione
 * @param {int} inizializzazione: la modalità di inizializzazione che deve avere l'individuo
 * che si sta avviando
 */
void avvia_individuo (caratteristiche_individuo individuo, int init_people, int inizializzazione);

/**
 * Crea casualmente le proprietà che dovrà avere il nuovo individuo e lo avvia.
 * 
 * @param {unsigned long} genes: Rappresenta fino a che numero un gene può arrivare, serve per
 * limitare il valore casuale
 * @param {int} init_people: il numero massimo d'individui all'interno della simulazione
 * @param {char} tipo: Il tipo dell'individuo che si sta creando che può essere A, B
 * oppure C in caso non sia da impostare
 */
caratteristiche_individuo crea_individuo (unsigned long genes, int init_people, char tipo);

/**
 * Inizializza init_people individui prima di cominciare la simulazione
 * 
 * @param {int} init_people: Il numero di individui da inizializzare
 * @param {unsigned long} genes: Il massimo valore che può assumere il genoma di un individuo.
 */
void inizializza_individui(int init_people, unsigned long genes);

/**
 * Inizializza l'array di rappresentazioni degli individui di un detrminato tipo
 * con dei valori per evitare quelli di default
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
 * @param {int} pid_terminatore_processi: Il pid del processo terminatore
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
 * @param {descrizione_simulazione} descrizione: La struttura che contiene le informazioni
 * inerenti la simulazione
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
 * @param {descrizione_individui*} descrizione: Una struttura che deve essere aggiornata
 * ad ogni terminazione e ad ogni creazione di un nuovo individuo.
 */
void attivita_terminatore_individui(int init_people, int birth_death, unsigned long genes, descrizione_simulazione* descrizione_simulazione);

/**
 * Chiamato quando un processo casuale deve terminare, il metodo lo sceglie, lo termina ed aggiorna la descrizione.
 * 
 * @param {int} init_people: La lunghezza massima che può avere l'array di individui
 * @param {descrizione_simulazione*} descrizione: La struttura che descrive la simulazione
 * @param {char} tipo_terminazione: Il tipo che andrà a terminare
 */
void preparazione_terminazione(int init_people, descrizione_simulazione* descrizione, char tipo_terminazione);

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
 * Recupera l'individuo con pid uguale al parametro "pid" dalla shm
 * 
 * @param {pid_t} pid: Il pid di un individuo di tipo A che si trova sulla shm A
 * @param {int} init_people: Il numero totale di individui
 * @param {descrizione_simulazione*} descrizione: La struttura che descrive la simulazione
 * @param {int} sem_shm_descrizione_id: L'ID del semaforo che regola l'accesso in mutua esclusione
 * @param {char} tipo: Il tipo da recuperare
 * 
 * @return {caratteristiche_individuo}: Le caratteristiche dell'individuo trovato
 * nella shm con PID uguale al valore passato come parametro. Individuo vuoto se
 * non è stato trovato l'individuo con il PID desiderato
 */
caratteristiche_individuo recupera(pid_t pid, int init_people, descrizione_simulazione* descrizione, int sem_shm_descrizione_id, char tipo);

/**
 * Calcola il massimo comun divisore tra a e b e lo restituisce
 * 
 * @param {unsigned long} a: Il primo numero da confrontare
 * @param {unsigned long} b: Il secondo numero da confrontare
 * 
 * @return il massimo comun divisore
 */
unsigned long mcd(unsigned long a, unsigned long b);

/**
 * Calcola il genoma che avrà il figlio della coppia
 * 
 * @param {unsigned long} genoma_individuo_a: il genoma dell'individuo A che si è accoppiato
 * @param {unsigned long} genoma_individuo_b: il genoma dell'individuo B che si è accoppiato
 * @param {unsigned long} genes: il valore di genes iniziale
 * 
 * @return Il genoma che avrà il figlio tra A e B
 */
unsigned long genoma_nuovo_da_coppia(unsigned long genoma_individuo_a, unsigned long genoma_individuo_b, unsigned long genes);

/**
 * Crea il nome che avrà il figlio di una coppia dopo l'accoppiamento e lo assegna al
 * parametro nome_nuovo_individuo
 * 
 * @param {bool} scelta_nome: Il genitore da cui viene derivato il nome del figlio
 * @param {char*} nome_nuovo_individuo: Il nome del nuovo individuo
 * @param {char*} nome_a: Il nome del genitore A
 * @param {char*} nome_b: Il nome del genitore B
 */
void nome_nuovo_da_coppia(bool scelta_nome, char* nome_nuovo_individuo, char* nome_a, char* nome_b);

/**
 * Crea le caratteristiche di un nuovo individuo che si sta per creare da una coppia
 * 
 * @param {caratteristiche_individuo*} nuovo_individuo: La struttura del nuovo individuo che si sta creando
 * @param {caratteristiche_individuo*} individuo_a: Le caratteristiche del genitore A
 * @param {caratteristiche_individuo*} individuo_b: Le caratteristiche del genitore B
 * @param {unsigned long} genes: il valore di genes iniziale
 * @param {int} scelta_nome: Viene utilizzato per stabilire il genitore da cui viene ereditato parte del nome
 * @param {rappresentazione_individuo*} individui_B: L'array contenente gli individui B
 * @param {rappresentazione_individuo*} individui_A: L'array contenente gli individui A
 * @param {int} init_people: Il numero totale di individui
 * @param {char*} tipo_scelto: il tipo che è stato precedentemente scelto C se non è stato ancora scelto
 * nessun tipo precedentemente
 */
void crea_individuo_da_coppia(caratteristiche_individuo* nuovo_individuo, caratteristiche_individuo individuo_a,caratteristiche_individuo individuo_b, unsigned long genes, bool scelta_nome, rappresentazione_individuo* individui_A, rappresentazione_individuo* individui_B, int init_people, char* tipo_scelto);

void aggiorna_utente_terminazione_simulazione();

#endif
