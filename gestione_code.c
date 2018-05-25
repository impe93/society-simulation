#include "gestione_code.h"

int msg_crea_coda_messaggi(int chiave) {
    int id = 0;
    if ((id = msgget(chiave, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        printf("Errore durante la creazione della coda per la chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

int msg_recupera_coda(int chiave) {
    int id = 0;
    if ((id = msgget(chiave, 0)) == -1) {
        printf("Errore durante il recupero della coda per la chiave %i.\n", chiave);
        exit(EXIT_FAILURE);
    }
    return id;
}

void msg_manda_messaggio(int id, char messaggio [], long tipo) {
    msg da_inviare;
    da_inviare.mtype = tipo;
    strcpy(da_inviare.mtext, messaggio);
    if (msgsnd(id, &da_inviare, sizeof(msg) - sizeof(long), 0) == -1) {
        printf("Errore durante l'invio del messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

void msg_manda_messaggio_individuo(int id, individuo_per_accoppiamento messaggio, long tipo) {
    msg_individuo da_inviare;
    da_inviare.mtype = tipo;
    da_inviare.individuo = messaggio;
    if (msgsnd(id, &da_inviare, sizeof(msg_individuo) - sizeof(long), 0) == -1) {
        printf("Errore durante l'invio del messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

void msg_manda_messaggio_accoppiamento(int id, bool esito_accoppiamento, long tipo) {
    msg_esito_accoppiamento da_inviare;
    da_inviare.mtype = tipo;
    da_inviare.esito = esito_accoppiamento;
    if (msgsnd(id, &da_inviare, sizeof(msg_esito_accoppiamento) - sizeof(long), 0) == -1) {
        printf("Errore durante l'invio del messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

void msg_manda_messaggio_notifica_accoppiamento(int id, informazioni_accoppiamento informazioni) {
    msg_notifica_accoppiamento da_inviare;
    da_inviare.mtype = 1;
    da_inviare.informazioni = informazioni;
    if (msgsnd(id, &da_inviare, sizeof(msg_notifica_accoppiamento) - sizeof(long), 0) == -1) {
        printf("Errore durante l'invio del messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}

void msg_ricevi_messaggio(int id, long tipo, char* messaggio_ricevuto) {
    msg da_ricevere;
    if (msgrcv(id, &da_ricevere, sizeof(msg) - sizeof(long), tipo, 0) == -1) {
        printf("Errore durante la ricezione di un messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
    strcpy(messaggio_ricevuto, da_ricevere.mtext);
}

void msg_ricevi_messaggio_individuo(int id, long tipo, individuo_per_accoppiamento* messaggio_ricevuto) {
    msg_individuo da_ricevere;
    if (msgrcv(id, &da_ricevere, sizeof(msg_individuo) - sizeof(long), tipo, 0) == -1) {
        printf("Errore durante la ricezione di un messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
    *messaggio_ricevuto = da_ricevere.individuo;
}

void msg_ricevi_messaggio_accoppiamento(int id, long tipo, bool* messaggio_ricevuto) {
    msg_esito_accoppiamento da_ricevere;
    if (msgrcv(id, &da_ricevere, sizeof(msg_esito_accoppiamento) - sizeof(long), tipo, 0) == -1) {
        printf("Errore durante la ricezione di un messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
    *messaggio_ricevuto = da_ricevere.esito;
}

void msg_ricevi_messaggio_notifica_accoppiamento(int id, informazioni_accoppiamento* informazioni) {
    msg_notifica_accoppiamento da_ricevere;
    if (msgrcv(id, &da_ricevere, sizeof(msg_notifica_accoppiamento) - sizeof(long), 0, 0) == -1) {
        if (!(errno == EINTR)) {
            ERRNO;
            DEBUG;
            printf("Errore durante la ricezione di un messaggio sulla coda con ID = %i.\n", id);
            exit(EXIT_FAILURE);
        }
    }
    *informazioni = da_ricevere.informazioni;
}

void msg_ricevi_messaggio_nowait(int id, long tipo, char* messaggio_ricevuto) {
    msg da_ricevere;
    if (msgrcv(id, &da_ricevere, sizeof(msg) - sizeof(long), tipo, IPC_NOWAIT) == -1) {
        printf("Errore durante la ricezione di un messaggio sulla coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
    strcpy(messaggio_ricevuto, da_ricevere.mtext);
}

bool msg_controlla_presenza_messaggi(int id, long tipo, individuo_per_accoppiamento* messaggio_ricevuto) {
    msg_individuo da_ricevere;
    if (msgrcv(id, &da_ricevere, sizeof(msg_individuo) - sizeof(long), tipo, IPC_NOWAIT) == -1) {
        if(errno == ENOMSG){
            return FALSE;
        } else{
            printf("Errore durante la ricezione di un messaggio sulla coda con ID = %i.\n", id);
            exit(EXIT_FAILURE);
        }
    }
    *messaggio_ricevuto = da_ricevere.individuo;
    return TRUE;
}

void msg_rimuovi_coda(int id) {
    if (msgctl(id, IPC_RMID, 0) == -1) {
        printf("Errore durante la rimozione della coda con ID = %i.\n", id);
        exit(EXIT_FAILURE);
    }
}