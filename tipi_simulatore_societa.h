#ifndef __TIPI_SIMULATORE_SOCIETA__

#define __TIPI_SIMULATORE_SOCIETA__

#include <stdio.h>

#define DEBUG printf("Sono arrivato alla linea %d del file %s.\n", __LINE__, __FILE__)

#define SHM_A_KEY 1234
#define SHM_B_KEY 1235

#define SEM_SINC_PADRE 1236
#define SEM_SINC_FIGLI 1237

typedef enum { FALSE, TRUE } bool;

/**
 * La struttura dati utilizzata per rappresentare un individuo
 */
typedef struct {
    char tipo;
    char nome [128];
    unsigned long genoma;
} caratteristiche_individuo;

/**
 * La struttura dati utilizzata per la memorizzazione nella shared memory
 */
typedef struct {
    bool utilizzata;
    pid_t pid;
    caratteristiche_individuo caratteristiche;
} rappresentazione_individuo;

#endif