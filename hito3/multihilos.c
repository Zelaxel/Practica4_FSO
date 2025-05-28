#include "sala.h"
#include "retardo.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define pausa 2

int terminado = 0;
int asientos[20]; // Asientos por liberar.
int indice = 0; // Indice de asientos por liberar.

// Muestra el estado de la sala cada cierto tiempo.
void* estado(void* n){
	while(!terminado){
		comprobar_asientos();
		pausa_aleatoria(pausa);
		printf("\n");
	}
	comprobar_asientos();
	pausa_aleatoria(pausa);
	printf("\n");
}

// Hilo que reserva asientos.
void* reservar(void* n){
	// Reservamos 3 asientos.
	for(int i=0; i<3; i++){
		// Reservamos i+1 por que los id de usuario no pueden ser < 1.
		if(reserva_asiento(i+1) == -1){ // Error al reservar.
			fprintf(stderr, "Error. No quedan asientos libres.\n");
		}
		pausa_aleatoria(pausa);
	}
}

// Hilo que libera asientos.
void* liberar(void* n){
	// Reservamos 3 asientos.
	for(int i=0; i<3; i++){
		// liberamos.
		if(libera_asiento() == -1){
			fprintf(stderr, "Error. No hay asientos por liberar.\n");
		}
		pausa_aleatoria(pausa);
	}
}

// Main que lanza N hilos.
int main(int argn, char* argv[]){
	// Comprobamos los argumentos.
	if(argn != 3){ // Error argumentos.
		fprintf(stderr, "Número de %d argumentos invalido. 2 requeridos.\n", argn-1);
		exit(-1);
	}

	int N = atoi(argv[1]); //Número de hilos a lanzar.
	int M = atoi(argv[2]);

	// Creamos la sala.
	if(crea_sala(20) == -1){ // Error crear sala.
		fprintf(stderr, "Error al crear la sala.\n");
		exit(-1);
	}

	pthread_t hilos_reserva[N]; // Hilos reserva.
	pthread_t hilos_liberacion[M]; // Hilos liberar.
	pthread_t hilo_estado; // Hilo que imprime la sala.

	// Lanzamos los hilos.
	pthread_create(&hilo_estado, NULL, estado, NULL);
	for(int i=0; i<N; i++){
		pthread_create(&hilos_reserva[i], NULL, reservar, NULL);
	}
	for(int i=0; i<M; i++){
		pthread_create(&hilos_liberacion[i], NULL, liberar, NULL);
	}

	// Esperamos a que terminen los hilos.
	void* dummy;
	for(int i=0; i<N; i++){
		pthread_join(hilos_reserva[i], &dummy);
	}
	for(int i=0; i<M; i++){
		pthread_join(hilos_liberacion[i], &dummy);
	}
	terminado = 1;
	pthread_join(hilo_estado, &dummy);

	elimina_sala();
	
	printf("Simulación terminada.\n");

	exit(0); // Salida con exito.
}
