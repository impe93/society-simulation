#ifndef _SEM_PROTOCOL_

#define _SEM_PROTOCOL_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <ctype.h>

#ifndef __APPLE__
union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
    
    #if defined(__linux__)
    struct seminfo* __buf;
    #endif
};
#endif

// Initialize semaphore to 1 (i.e., "available")
int initSemAvailable(int semId, int semNum);

// Initialize semaphore to 0 (i.e., "in use")
int initSemInUse(int semId, int semNum);

// Reserve semaphore - decrement it by 1
int reserveSem(int semId, int semNum);

// Release semaphore - increment it by 1
int releaseSem(int semId, int semNum);

#endif
