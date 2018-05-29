socety-simulation: tipo_A tipo_B gestore

funzioni_gestore.o: funzioni_gestore.c funzioni_gestore.h tipi_simulatore_societa.h gestione_semafori.h gestione_shm.h sem_protocol.h
	gcc -Wall -pedantic -c funzioni_gestore.c

funzioni_A_B.o: funzioni_A_B.c funzioni_A_B.h tipi_simulatore_societa.h
	gcc -Wall -pedantic -c funzioni_A_B.c

funzioni_B.o: funzioni_B.c funzioni_B.h gestione_code.h tipi_simulatore_societa.h funzioni_A_B.h
	gcc -Wall -pedantic -c funzioni_B.c

gestione_code.o: gestione_code.c gestione_code.h tipi_simulatore_societa.h
	gcc -Wall -pedantic -c gestione_code.c

gestione_semafori.o: gestione_semafori.c gestione_semafori.h sem_protocol.h
	gcc -Wall -pedantic -c gestione_semafori.c

sem_protocol.o: sem_protocol.c sem_protocol.h
	gcc -Wall -pedantic -c sem_protocol.c

gestione_shm.o: gestione_shm.c gestione_shm.h tipi_simulatore_societa.h
	gcc -Wall -pedantic -c gestione_shm.c

tipo_A.o: tipo_A.c tipi_simulatore_societa.h gestione_code.h gestione_semafori.h gestione_shm.h funzioni_A_B.h sem_protocol.h
	gcc -Wall -pedantic -c tipo_A.c

tipo_A: tipo_A.o gestione_code.o gestione_semafori.o gestione_shm.o funzioni_A_B.o sem_protocol.o
	gcc -Wall -pedantic -o tipo_A tipo_A.o gestione_code.o gestione_semafori.o gestione_shm.o funzioni_A_B.o sem_protocol.o

tipo_B.o: tipo_B.c tipi_simulatore_societa.h gestione_code.h gestione_semafori.h gestione_shm.h funzioni_A_B.h funzioni_B.h sem_protocol.h
	gcc -Wall -pedantic -c tipo_B.c

tipo_B: tipo_B.o gestione_code.o gestione_semafori.o gestione_shm.o funzioni_A_B.o funzioni_B.o sem_protocol.o
	gcc -Wall -pedantic -o tipo_B tipo_B.o gestione_code.o gestione_semafori.o gestione_shm.o funzioni_A_B.o funzioni_B.o sem_protocol.o

gestore.o: gestore.c tipi_simulatore_societa.h funzioni_gestore.h gestione_code.h gestione_semafori.h gestione_shm.h sem_protocol.h
	gcc -Wall -pedantic -c gestore.c

gestore: gestore.o funzioni_gestore.o gestione_code.o gestione_semafori.o gestione_shm.o sem_protocol.o
	gcc -Wall -pedantic -o gestore gestore.o funzioni_gestore.o gestione_code.o gestione_semafori.o gestione_shm.o sem_protocol.o

clean:
	rm -f *.o
	rm gestore tipo_A tipo_B
