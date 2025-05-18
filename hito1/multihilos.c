#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "sala.h"
#include "retardo.h"

#define ITERACIONES_HILO 3
#define MAX_PAUSA 0.5
#define CAPACIDAD_SALA 20

void* hilo_reservas(void* args) {
	int asientos[ITERACIONES_HILO];
	for(int i = 0; i < ITERACIONES_HILO; i++) {
		asientos[i] = reserva_asiento(i+20);
		pausa_aleatoria(MAX_PAUSA);
	}
	
	for(int i = 0; i < ITERACIONES_HILO; i++) {
		libera_asiento(asientos[i]);
		pausa_aleatoria(MAX_PAUSA);
	}
}

void* hilo_estado(void* args) {
	while(1) {
		comprobar_asientos();
		pausa_aleatoria(MAX_PAUSA);
	}
}


int ret;
int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Uso: ./ numero_de_hilos\n");
		exit(-1);
	}
	
	
	int n_threads = atoi(argv[1]);
	
	pthread_t * threads;
	if ((threads=malloc(n_threads*sizeof(pthread_t))) == NULL) {
		fprintf(stderr, "Error al asignar memoria a los hilos: %s\n", strerror(errno));
		exit(-1);
	}
	
	if (crea_sala(CAPACIDAD_SALA) == -1) {
		fprintf(stderr, "Error al crear la sala.\n");
		exit(-1);
	}
	
	for (int i = 0; i < n_threads; i++) {
		if (ret = pthread_create(&threads[i], NULL, hilo_reservas, NULL)) {
			errno = ret;
			fprintf(stderr, "Error al crear el hilo %d: %s\n", i, strerror(errno));
		}
	}
	
	pthread_t state_thread;
	if (ret = pthread_create(&state_thread, NULL, hilo_estado, NULL)) {
		errno = ret;
		fprintf(stderr, "Error al crear el hilo estado: %s\n", strerror(errno));
		exit(-1);
	}
	
	for (int i = 0; i < n_threads; i++) {
		if (ret = pthread_join(threads[i], NULL)) {
			errno = ret;
			fprintf(stderr, "Error al crear el hilo %d: %s\n", i, strerror(errno));
			exit(-1);
		}
	}
	
	pthread_cancel(state_thread);
	
	comprobar_asientos();
	elimina_sala();
	
	free(threads);
	
	exit(0);
}
