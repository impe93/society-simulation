#ifndef __TIPI_SIMULATORE_SOCIETA__

#define __TIPI_SIMULATORE_SOCIETA__

#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define DEBUG printf("Sono arrivato alla linea %d del file %s con PID %i.\n", __LINE__, __FILE__, getpid())
#define ERRNO printf("Error number = %i\n", errno)



#define SHM_A_KEY 1234
#define SHM_B_KEY 1235
#define SHM_DESCRIZIONE_KEY 1243

#define SEM_SINC_GESTORE 1236
#define SEM_SINC_INDIVIDUI 1237
#define SEM_SHM_A 1238
#define SEM_SHM_B 1239
#define SEM_SHM_DESCRIZIONE 1244

#define MSG_A_B 1240
#define MSG_GESTORE_A 1241
#define MSG_GESTORE_B 1242

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

/**
 * Struttura utilizzata per tracciare i cambiamenti durante la simulazione
 */
typedef struct {
    int individui_a_creati;
    int individui_b_creati;
    caratteristiche_individuo individuo_nome_lungo;
    caratteristiche_individuo individuo_genoma_lungo;
    int individui_a_attivi;
    int individui_b_attivi;
    int processi_totali_terminati;
} descrizione_simulazione;

#endif