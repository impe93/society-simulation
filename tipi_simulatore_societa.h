#ifndef __TIPI_SIMULATORE_SOCIETA__

#define __TIPI_SIMULATORE_SOCIETA__

#include <stdio.h>

#define DEBUG printf("Sono arrivato alla linea %d del file %s.\n", __LINE__, __FILE__)

#define SHM_A_KEY 1234

typedef enum { FALSE, TRUE } bool;

typedef struct {
    char tipo;
    char nome [128];
    unsigned long genoma;
} caratteristiche_individuo;

typedef struct {
    bool utilizzata;
    pid_t pid;
    caratteristiche_individuo caratteristiche;
} rappresentazione_individuo;

#endif