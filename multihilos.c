#include "sala.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define capacidad = 500

// Reserva y liberación de 3 asientos.
void* ejecucion(void* n){
	
	int asientos[3]; // ID de los asientos reservados.
	
	// Reservamos 3 asientos.
	for(int i=0; i<3, i++){
		asientos[i] = reserva_asiento(i+1); // i+1 por que los id de usuario no pueden ser = 0.
		sleep(0.5);
	}

	// Reservamos 3 asientos.
	for(int i=0; i<3, i++){
		libera_asiento(asientos[i]); // liberamos.
		sleep(0.5);
	}
}

// Main que lanza N hilos.
int main(int argn, char* arngv[]){
	// Comprobamos los argumentos.
	if(argn != 2){ // Error argumentos.
		fprintf(stderr, "Número de %d argumentos invalido. 1 requerido.", argn-1);
		exit(-1);
	}
	
	// Creamos la sala.
	if(crea_sala(capacidad) == -1){ // Error crear sala.
		fprintf(stderr, "Error al crear la sala.");
		exit(-1);
	}
	
	int N = atoi(argv[1]); //Número de hilos a lanzar.
	pthread_t hilos[N]; // Hilos.
	
	// Lanzamos los hilos.
	for(int i=0; i<N; i++){
		pthread_create(hilos[i], NULL, ejecucion, NULL);	
	}
	
	// Esperamos a que terminen los hilos.
	for(int i=0; i<N; i++){
		void* dummy;
		pthread_join(hilos[i], dummy);	
	}
	
	exit(0);
}
