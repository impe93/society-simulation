#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "tipi_simulatore_societa.h"
#include "funzioni_gestore.h"
#include "gestione_code.h"
#include "gestione_semafori.h"
#include "gestione_shm.h"

int main(int argc, char** argv) {
    
    /**
     * Un int che rappresenta il numero della popolazione che verrà generata all'inizio
     */
    unsigned int init_people = 0;

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
     * Un int che rappresenta i secondi di esecuzione della simulazione.
     */
    unsigned int sim_time = 0;

    
    
    srand(time(NULL));

    // Assegnamento delle variabili passate come parametro
    if (argc == 5) {
        bool corretto = TRUE;

        if (isUnsignedNumber(*(argv + 1))) {
            init_people = atoi(*(argv + 1));
        } else {
            printf("Il numero che rappresenta la popolazione deve essere un intero non segnato.\n");
            corretto = FALSE;
        }

        if (isUnsignedNumber(*(argv + 2))) {
            genes = atol(*(argv + 2));
        } else {
            printf("Il numero che rappresenta il gene deve essere un long non segnato.\n");
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

    inizializza_individui(init_people, genes);

}