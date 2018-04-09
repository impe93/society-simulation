#ifndef __TIPI_SIMULATORE_SOCIETA__

#define __TIPI_SIMULATORE_SOCIETA__

#include <stdio.h>

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