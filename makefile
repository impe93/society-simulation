socety-simulation: tipo_A tipo_B gestore

tipo_A: tipo_A.c tipi_simulatore_societa.h gestione_code.h gestione_semafori.h gestione_shm.h funzioni_A_B.h sem_protocol.h
	gcc tipo_A.c -o tipo_A

tipo_B: tipo_B.c tipi_simulatore_societa.h gestione_code.h gestione_semafori.h gestione_shm.h funzioni_A_B.h funzioni_B.h sem_protocol.h
	gcc tipo_B.c -o tipo_B

gestore: gestore.c sem_protocol.h gestione_shm.h gestione_semafori.h tipi_simulatore_societa.h gestione_code.h funzioni_gestore.h
	gcc gestore.c -o gestore

clean:
	rm -f *.o
	rm gestore tipo_A tipo_B
