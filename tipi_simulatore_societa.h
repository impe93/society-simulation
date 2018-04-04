#ifndef __TIPI_SIMULATORE_SOCIETA__

#define __TIPI_SIMULATORE_SOCIETA__

#include <stdio.h>

typedef enum { FALSE, TRUE } bool;

typedef struct {
    char tipo [1];
    char nome [128];
    unsigned long genoma;
} caratteristiche_individuo;

typedef struct {
    pid_t pid;
    caratteristiche_individuo caratteristiche;
} rappresentazione_individuo;

#endif