#include "funzioni_A_B.h"

void inserimento_caratteristiche_individuo(caratteristiche_individuo* p, char** argv){
  (*p).tipo = *argv[0];
  strcpy(p->nome, *(argv + 1));
  (*p).genoma = atol(*(argv + 2));
}

void inserimento_in_shm(rappresentazione_individuo* p_shm, pid_t pid, caratteristiche_individuo individuo, int numero_individui){
  bool inserito_in_shm = FALSE;
  for(int i = 0; inserito_in_shm == FALSE && i < numero_individui; i++){
    if(p_shm[i].utilizzata == FALSE){
      p_shm[i].pid = pid;
      p_shm[i].caratteristiche = individuo;
      p_shm[i].utilizzata = TRUE;
      inserito_in_shm = TRUE;
    }
  }
}

unsigned long mcd(unsigned long a, unsigned long b){
  int r = 0;
  while(b != 0) {
    r = a % b;
    a = b; 
    b = r;
  }
  return a;
}

void rimozione_da_shm(rappresentazione_individuo* p_shm, pid_t pid, int numero_individui){
  bool rimosso = FALSE;
  for(int i = 0; rimosso == FALSE && i < numero_individui; i++){
    if(p_shm[i].pid == pid){
      p_shm[i].utilizzata = FALSE;
      rimosso = TRUE;
    }
  }
}
