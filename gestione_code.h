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

/**
 * La struttura dati utilizzata per lo scambio di messaggi
 */
typedef struct {
    long mtype;
    char mtext[256];
} msg;


/**
 * Crea una nuova coda di messaggi utilizzando come chiave il parametro "chiave" e restituisce
 * l'ID della coda appena creata.
 * 
 * @param chiave: E la chiave utilizzata per la creazione della nuova coda
 * @return: ritorna l'ID della coda appena creata, in caso di errore durante la creazione
 * chiude il programma
 */
int msg_crea_coda_messaggi(int chiave) {
    int id = 0;
    if ((id = msgget(chiave, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        printf("Errore durante la creazione della coda per la chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * Recupera l'ID di una coda già creata dalla chiave del parametro "chiave" e ne restituisce
 * l'ID.
 * 
 * @param chiave: E la chiave di una coda già esistente
 * @return: ritorna l'ID della coda appena creata, in caso di errore durante il recupero
 * chiude il programma
 */
int msg_recupera_coda(int chiave) {
    int id = 0;
    if ((id = msgget(chiave, 0)) == -1) {
        printf("Errore durante il recupero della coda per la chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * Manda un messaggio uguale al parametro "Messaggio" ad una coda con ID uguale al
 * parametro "id" e di tipo uguale al parametro "tipo", in caso di errore durante
 * l'invio il programma termina.
 * 
 * @param id: L'ID della coda su cui si vuole mandare il messaggio
 * @param messaggio: Una stringa che indica il messaggio che si vuole mandare
 * @param tipo: Un long che indice il tipo del messaggio che si vuole mandare
 */
void msg_manda_messaggio(int id, char messaggio [], long tipo) {
    msg da_inviare;
    da_inviare.mtype = tipo;
    strcpy(da_inviare.mtext, messaggio);
    if (msgsnd(id, &da_inviare, sizeof(msg) - sizeof(long), 0) == -1) {
        printf("Errore durante l'invio del messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

/**
 * Riceve un messaggio dalla coda con ID uguale al parametro "id" e di tipo uguale
 * al parametro "tipo", la stringa del messaggio ricevuto viene copiato all'interno
 * del parametro "messaggio_ricevuto", in caso di fallimento invece conclude il processo.
 * 
 * @param id: Un int che rappresenta l'ID della coda da cui si vuole ricevere il messaggio.
 * @param tipo: Un long che rappresenta il tipo del messaggio che si vuole ricevere.
 * @param messaggio_ricevuto: Un puntatore a char che rappresenta la stringa ricevuta,
 * inizialmente vuota.
 */
void msg_ricevi_messaggio(int id, long tipo, char* messaggio_ricevuto) {
    msg da_ricevere;
    if (msgrcv(id, &da_ricevere, sizeof(msg) - sizeof(long), tipo, 0) == -1) {
        printf("Errore durante la ricezione di un messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
    strcpy(messaggio_ricevuto, da_ricevere.mtext);
}

/**
 * Riceve un messaggio dalla coda con ID uguale al parametro "id" e di tipo uguale
 * al parametro "tipo", la stringa del messaggio ricevuto viene copiato all'interno
 * del parametro "messaggio_ricevuto", in caso non ci siano messaggi non attende,
 * in caso di fallimento invece conclude il processo.
 * 
 * @param id: Un int che rappresenta l'ID della coda da cui si vuole ricevere il messaggio.
 * @param tipo: Un long che rappresenta il tipo del messaggio che si vuole ricevere.
 * @param messaggio_ricevuto: Un puntatore a char che rappresenta la stringa ricevuta,
 * inizialmente vuota.
 */
void msg_ricevi_messaggio_nowait(int id, long tipo, char* messaggio_ricevuto) {
    msg da_ricevere;
    if (msgrcv(id, &da_ricevere, sizeof(msg) - sizeof(long), tipo, 0) == -1) {
        printf("Errore durante la ricezione di un messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
    strcpy(messaggio_ricevuto, da_ricevere.mtext);
}

/**
 * Rimuove la coda con ID uguale al paramentro "id", in caso di fallimento termina il processo.
 * 
 * @param id: Un intero che rappresenta l'ID della coda che si vuole rimuovere.
 */
void msg_rimuovi_coda(int id) {
    if (msgctl(id, IPC_RMID, 0) == -1) {
        printf("Errore durante la rimozione della coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

#endif