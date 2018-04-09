#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>

#include "gestione_code.h"
#include "gestione_semafori.h"
#include "gestione_shm.h"
#include "funzioni_A.h"

int main(int argc, char** argv){
    /*
    Recupero dei parametri passati dal gestore per la creazione (tipo, nome, genoma)
    */
    caratteristiche_individuo individuo;
    if(argc == 4) {
        inserimento_caratteristiche_individuo(&individuo, argv);
    } else {
        printf("numero di argomenti passati al processo A errato\n");
        exit(EXIT_FAILURE);
    }
    /*
    printf("contenuto di individuo: \n");
    printf("tipo: %c\n", individuo.tipo);
    printf("nome: %s\n", individuo.nome);
    printf("genoma: %ul\n", individuo.genoma);
    */
   

    exit(EXIT_SUCCESS);
}