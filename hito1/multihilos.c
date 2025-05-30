#include "sala.h"
#include "retardo.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define pausa 1

int terminado = 0;

// Muestra el estado de la sala cada cierto tiempo.
void* estado(void* n){
	int iter = 0;
	while(!terminado){
		printf("\nVista %d\n", iter);
		comprobar_asientos();
		printf("\n");
		sleep(1);
		iter++;
	}
	printf("\nVista %d\n", iter);
	comprobar_asientos();
	printf("\n");
}

// Reserva y liberación de 3 asientos.
void* ejecucion(void* n){

	int n_asientos_reservados = 0;

	// Reservamos 3 asientos.
	for(int i=0; i<3; i++){
		// Reservamos i+1 por que los id de usuario no pueden ser < 1.
		if(reserva_asiento(i+1) == -1){ // Error al reservar.
			fprintf(stderr, "Error. No quedan asientos libres.\n");
			break;
		}
		n_asientos_reservados++;
		pausa_aleatoria(pausa);
	}

	// Liberamos 3 asientos.
	for(int i=0; i<n_asientos_reservados; i++){
		// liberamos.
		if(libera_asiento() == -1){ // En caso de error el progama continua hasta liberar los asientos.
			fprintf(stderr, "Error. No quedan asientos por liberar.\n");
		}
		pausa_aleatoria(pausa);
	}
}

// Main que lanza N hilos.
int main(int argn, char* argv[]){
	// Comprobamos los argumentos.
	if(argn != 2){ // Error argumentos.
		fprintf(stderr, "Número de %d argumentos invalido. 1 requerido.\n", argn-1);
		exit(-1);
	}

	int N = atoi(argv[1]); //Número de hilos a lanzar.

	// Creamos la sala.
	if(crea_sala(20) == -1){ // Error crear sala.
		fprintf(stderr, "Error al crear la sala.\n");
		exit(-1);
	}

	pthread_t hilos[N]; // Hilos.
	pthread_t hilo_estado; // Hilo que imprime la sala.

	// Lanzamos los hilos.
	pthread_create(&hilo_estado, NULL, estado, NULL);
	for(int i=0; i<N; i++){
		pthread_create(&hilos[i], NULL, ejecucion, NULL);
	}

	// Esperamos a que terminen los hilos.
	void* dummy;
	for(int i=0; i<N; i++){
		pthread_join(hilos[i], &dummy);
	}
	terminado = 1;
	pthread_join(hilo_estado, &dummy);

	elimina_sala();
	
	printf("Simulación terminada.\n");

	exit(0); // Salida con exito.
}
