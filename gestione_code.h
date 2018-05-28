#ifndef __GESTIONE_CODE__

#define __GESTIONE_CODE__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "tipi_simulatore_societa.h"

/**
 * La struttura dati utilizzata per rappresentare messaggi standard.
 */
typedef struct {
    long mtype;
    char mtext[256];
} msg;

/**
 * La struttura dati utilizzata per rappresentare un individuo che si 
 * vuole accoppiare.
 */
typedef struct {
    pid_t pid;
    caratteristiche_individuo caratteristiche;
} individuo_per_accoppiamento;

/**
 * La struttura dati utilizzata per rappresentare un messaggio di richiesta 
 * di accoppiamento.
 */
typedef struct {
    long mtype;
    individuo_per_accoppiamento individuo;
} msg_individuo;

/**
 * La struttura dati utilizzata per rappresentare un messaggio di risposta ad
 * una richiesta di accoppiamento.
 */
typedef struct {
    long mtype;
    bool esito;
} msg_esito_accoppiamento;

/**
 * La struttura dati utilizzata per rappresentare le informazioni riguardanti
 * un avvenuto accoppiamento.
 */
typedef struct {
    char tipo_mittente;
    pid_t pid_mittente;
    pid_t pid_coniuge;
} informazioni_accoppiamento;

/**
 * La struttura dati utilizzata per rappresentare un messaggio di notifica al
 * gestore di un avvenuto accoppiamento da parte di un individuo A oppure B. 
 */
typedef struct {
    long mtype;
    informazioni_accoppiamento informazioni;
} msg_notifica_accoppiamento;

/**
 * Crea una nuova coda di messaggi utilizzando come chiave il parametro "chiave" e restituisce
 * l'ID della coda appena creata.
 * @param {int} chiave: la chiave utilizzata per la creazione della nuova coda
 * @return {int}: ritorna l'ID della coda appena creata, in caso di errore durante la creazione
 * chiude il programma
 */
int msg_crea_coda_messaggi(int chiave) ;

/**
 * Recupera l'ID di una coda già creata dalla chiave del parametro "chiave" e ne restituisce
 * l'ID.
 * @param {int} chiave: la chiave di una coda già esistente
 * @return {int}: ritorna l'ID della coda appena creata, in caso di errore durante il recupero
 * chiude il programma
 */
int msg_recupera_coda(int chiave);

/**
 * Manda un messaggio uguale al parametro "Messaggio" ad una coda con ID uguale al
 * parametro "id" e di tipo uguale al parametro "tipo", in caso di errore durante
 * l'invio il programma termina.
 * @param {int} id: L'ID della coda su cui si vuole mandare il messaggio
 * @param {char[]} messaggio: Una stringa che indica il messaggio che si vuole mandare
 * @param {long} tipo: Un long che indica il tipo del messaggio che si vuole mandare
 */
void msg_manda_messaggio(int id, char messaggio [], long tipo);

/**
 * Manda un messaggio con contenuto corrispondente al parametro "messaggio" ad una coda con ID 
 * uguale al parametro "id" e di tipo uguale al parametro "tipo", in caso di errore durante
 * l'invio il programma termina.
 * @param {int} id: L'ID della coda su cui si vuole mandare il messaggio
 * @param {individuo_per_accoppiamento} messaggio: Una struct individuo_per_accoppiamento che 
 * indica il messaggio che si vuole mandare
 * @param {long} tipo: Un long che indica il tipo del messaggio che si vuole mandare
 */
void msg_manda_messaggio_individuo(int id, individuo_per_accoppiamento messaggio, long tipo) ;

/**
 * Manda un messaggio con contenuto corrispondente al parametro "esito_accoppiamento" ad una coda con ID 
 * uguale al parametro "id" e di tipo uguale al parametro "tipo", in caso di errore durante
 * l'invio il programma termina.
 * @param {int} id: L'ID della coda su cui si vuole mandare il messaggio
 * @param {bool} esito_accoppiamento: valore di tipo bool che indica se l'accoppiamento è avvenuto
 * oppure no, costituisce il contenuto del messaggio
 * @param {long} tipo: Un long che indica il tipo del messaggio che si vuole mandare
 */
void msg_manda_messaggio_accoppiamento(int id, bool esito_accoppiamento, long tipo);

/**
 * Manda un messaggio con contenuto corrispondente al parametro "informazioni" ad una coda con ID 
 * uguale al parametro "id", in caso di errore durante l'invio il programma termina.
 * @param {int} id: L'ID della coda su cui si vuole mandare il messaggio
 * @param {informazioni_accoppiamento} informazioni: contiene il pid, il tipo del processo chiamante 
 * e il pid del processo con il quale esso si è accoppiato.
 */
void msg_manda_messaggio_notifica_accoppiamento(int id, informazioni_accoppiamento informazioni); 

/**
 * Riceve un messaggio dalla coda con ID uguale al parametro "id" e di tipo uguale
 * al parametro "tipo", la stringa del messaggio ricevuto viene copiato all'interno
 * del parametro "messaggio_ricevuto", in caso di fallimento invece conclude il processo.
 * @param {int} id: Un int che rappresenta l'ID della coda da cui si vuole ricevere il messaggio.
 * @param {long} tipo: Un long che rappresenta il tipo del messaggio che si vuole ricevere.
 * @param {char*} messaggio_ricevuto: Un puntatore a char che rappresenta la stringa ricevuta,
 * inizialmente vuota.
 */
void msg_ricevi_messaggio(int id, long tipo, char* messaggio_ricevuto);

/**
 * Riceve un messaggio dalla coda con ID uguale al parametro "id" e di tipo uguale
 * al parametro "tipo", la struct del messaggio ricevuto viene copiata all'interno
 * del parametro "messaggio_ricevuto", in caso di fallimento invece conclude il processo.
 * @param {int} id: Un int che rappresenta l'ID della coda da cui si vuole ricevere il messaggio.
 * @param {long} tipo: Un long che rappresenta il tipo del messaggio che si vuole ricevere.
 * @param {individuo_per_accoppiamento} messaggio_ricevuto: Un puntatore individuo_per_accoppiamento 
 * che rappresenta le informazioni su un individuo ricevute, inizialmente nullo.
 */
void msg_ricevi_messaggio_individuo(int id, long tipo, individuo_per_accoppiamento* messaggio_ricevuto);

/**
 * Riceve un messaggio dalla coda con ID uguale al parametro "id" e di tipo uguale
 * al parametro "tipo", il valore contenuto nel campo esito del messaggio ricevuto viene
 * copiato all'interno del parametro "messaggio_ricevuto", in caso di fallimento invece 
 * conclude il processo.
 * @param {int} id: Un int che rappresenta l'ID della coda da cui si vuole ricevere il messaggio.
 * @param {long} tipo: Un long che rappresenta il tipo del messaggio che si vuole ricevere.
 * @param {bool*} messaggio_ricevuto: Un puntatore a bool che rappresenta la risposta alla richiesta 
 * di accoppiamento, inizialmente nullo.
 */
void msg_ricevi_messaggio_accoppiamento(int id, long tipo, bool* messaggio_ricevuto);

/**
 * Riceve un messaggio dalla coda con ID uguale al parametro "id", le informazioni contenute nel
 * messaggio vengono copiate all'interno del parametro "informazioni", in caso di fallimento 
 * invece conclude il processo.
 * @param {int} id: Un int che rappresenta l'ID della coda da cui si vuole ricevere il messaggio.
 * @param {informazioni_accoppiamento} informazioni: Un puntatore a informazioni_accoppiamento dove
 * vengono inserite le informazioni sui processi che si sono accoppiati, inizialmente nullo.
 */
void msg_ricevi_messaggio_notifica_accoppiamento(int id, informazioni_accoppiamento* informazioni);

/**
 * Riceve un messaggio dalla coda con ID uguale al parametro "id" e di tipo uguale
 * al parametro "tipo", la stringa del messaggio ricevuto viene copiato all'interno
 * del parametro "messaggio_ricevuto", in caso non ci siano messaggi non attende,
 * in caso di fallimento invece conclude il processo.
 * @param {int} id: Un int che rappresenta l'ID della coda da cui si vuole ricevere il messaggio.
 * @param {long} tipo: Un long che rappresenta il tipo del messaggio che si vuole ricevere.
 * @param {char*} messaggio_ricevuto: Un puntatore a char che rappresenta la stringa ricevuta,
 * inizialmente vuota.
 */
void msg_ricevi_messaggio_nowait(int id, long tipo, char* messaggio_ricevuto);

/**
 * Riceve un messaggio dalla coda con ID uguale al parametro "id" e di tipo uguale al parametro 
 * "tipo", la struct del messaggio ricevuto viene copiata all'interno del parametro 
 * "messaggio_ricevuto" e ritorna TRUE, se invece non ci sono messaggi con quel "tipo"
 * ritorna FALSE, in caso di fallimento invece conclude il processo.
 * @param {int} id: Un int che rappresenta l'ID della coda da cui si vuole ricevere il messaggio.
 * @param {long} tipo: Un long che rappresenta il tipo del messaggio che si vuole ricevere.
 * @param {individuo_per_accoppiamento} messaggio_ricevuto: Un puntatore individuo_per_accoppiamento 
 * che rappresenta le informazioni ricevute su un individuo, inizialmente nullo
 * @return {bool}: ritorna TRUE in caso abbia trovato un messaggio con quel tipo sulla coda,
 * FALSE altrimenti.
 */
bool msg_controlla_presenza_messaggi(int id, long tipo, individuo_per_accoppiamento* messaggio_ricevuto);

/**
 * Rimuove la coda con ID uguale al paramentro "id", in caso di fallimento termina il processo.
 * @param {int} id: Un intero che rappresenta l'ID della coda che si vuole rimuovere.
 */
void msg_rimuovi_coda(int id);

#endif
