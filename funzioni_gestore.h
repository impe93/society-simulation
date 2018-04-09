#ifndef __FUNZIONI_GESTORE__

#define __FUNZIONI_GESTORE__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>

#include "tipi_simulatore_societa.h"
#include "gestione_semafori.h"

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
void avvia_individuo (caratteristiche_individuo individuo);

/**
 * Crea casualmente le proprietà che dovrà avere il nuovo individuo e lo avvia.
 * 
 * @param {unsigned long} genes: Rappresenta fino a che numero un gene può arrivare, serve per
 * limitare il valore casuale
 */
void crea_individuo (unsigned long genes);

/**
 * Inizializza tanti individui in base a quanto è segnato nel parametro init_people
 * 
 * @param {int} init_people: Il numero di individui da inizializzare
 * @param {unsigned long} genes: Il massimo valore che può assumere il genoma di un individuo.
 * @param {pid_t []} individui_da_avviare: Ad ogni creazione di un individuo il suo pid viene messo
 * all'interno di questo array.
 */
void inizializza_individui(int init_people, unsigned long genes);

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

void avvia_individuo (caratteristiche_individuo individuo) {
    pid_t pid_figlio = 0;
    switch(pid_figlio = fork()) {
        case -1: {
            printf("Errore durante la creazione di un nuovo figlio.\n");
            exit(EXIT_FAILURE);
        }
        case 0: {
            char stringa_genoma [20];
            sprintf(stringa_genoma, "%ld", individuo.genoma);
            
            if (execl("./tipo_A", &individuo.tipo, individuo.nome, stringa_genoma, NULL) == -1) {
                printf("Errore durante la creazione del nuovo individuo.\n");
                exit(EXIT_FAILURE);
            }
            /* ---------- BLOCCO SOLO PER TEST ----------
            sem_rilascia(sem_recupero(SEM_SINC_PADRE));
            sem_riserva(sem_recupero(SEM_SINC_FIGLI));
            printf("Individuo tipo: %c, nome: %s, Genoma: %s\n", individuo.tipo, individuo.nome, stringa_genoma);
            exit(EXIT_SUCCESS);
            */
        }
        default: {
            break;
        }
    }
}

void crea_individuo (unsigned long genes) {
    caratteristiche_individuo individuo;

    if (rand() % 2) {
        individuo.tipo = 'A';
    } else {
        individuo.tipo = 'B';
    }
    individuo.genoma = (rand() % (genes + 1)) + 2;
    individuo.nome[0] = (char)(rand() % 25) + 65;

    avvia_individuo(individuo);
}

void inizializza_individui(int init_people, unsigned long genes) {
    for(int i = 0; i < init_people; i++) {
        crea_individuo(genes);
    }
}

/**
 * Inizializza l'array di rappresentazioni degli individui di un detrminato tipo
 * con valori di default
 * 
 * @param {int} shm_id: L'ID della shared memory di individui da inizializzare
 * @param {int} init_people: Il numero di individui totali che saranno presenti
 * all'interno del programma
 */
void inizializza_shm(int shm_id, int init_people) {
    rappresentazione_individuo** individui;
    shm_attach(shm_id, individui);
    for (int i = 0; i < init_people - 1; i++) {
        (*(*(individui + i))).utilizzata = FALSE;
        (*(*(individui + i))).pid = 0;
        (*(*(individui + i))).caratteristiche.tipo = 'C';
        (*(*(individui + i))).caratteristiche.genoma = 0;
        strcpy((*(*(individui + i))).caratteristiche.nome, "");
    }
    shm_detach(individui);
}

#endif
