#include "funzioni_B.h"

void seleziona_individuo_A_ideale(rappresentazione_individuo* p_shm_A, individuo_per_accoppiamento* individuo_A, 
                    unsigned long genoma_B, int numero_A){
    int indice_A_ideale = 0;
    int genoma_massimo = 0;
    for(int i = 0; i < numero_A; i++){
        if(p_shm_A[i].utilizzata == TRUE){
            int mcd_genomi = 0;
            if (p_shm_A[i].caratteristiche.genoma > genoma_B) {
                mcd_genomi = mcd(p_shm_A[i].caratteristiche.genoma, genoma_B);
            } else {
                mcd_genomi = mcd(genoma_B, p_shm_A[i].caratteristiche.genoma);
            }
            if(mcd_genomi >= genoma_massimo){
                genoma_massimo = mcd_genomi;
                indice_A_ideale = i;
            }
        }
    }
    (*individuo_A).caratteristiche = p_shm_A[indice_A_ideale].caratteristiche;
    (*individuo_A).pid = p_shm_A[indice_A_ideale].pid;
}

void inserimento_in_messaggio_accoppiamento(individuo_per_accoppiamento* messaggio_accoppiamento, 
                    pid_t pid_B, caratteristiche_individuo individuo_B){
    (*messaggio_accoppiamento).pid = pid_B;
    (*messaggio_accoppiamento).caratteristiche = individuo_B;
}