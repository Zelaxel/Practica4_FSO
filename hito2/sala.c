#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "retardo.h"

#define pausa 1

int* sala_teatro = NULL;
int capacidad_total = 0;
pthread_mutex_t cerrojo = PTHREAD_MUTEX_INITIALIZER;

int reserva_asiento(int id_persona){
	//Falla si la sala no esta creada o si el id de la persona no es valido.
	if(sala_teatro==NULL || id_persona < 1) return -1;

	//Busca un espacio libre.
	for(int i=0; i<capacidad_total; i++){
		if(sala_teatro[i] == -1){
			pausa_aleatoria(pausa);
			pthread_mutex_lock(&cerrojo);
			sala_teatro[i] = id_persona;
			pthread_mutex_unlock(&cerrojo);
			return i;
		}
	}
	return -1; //No hay espacio libre.
}

int libera_asiento(int id_asiento){
	//Falla si la sala no esta creada o si el id del asiento se sale del espacio.
	if(sala_teatro==NULL || id_asiento >= capacidad_total || id_asiento < 0) return -1;
	
	pausa_aleatoria(pausa);

	pthread_mutex_lock(&cerrojo);
	//Hay asiento por lo que lo libera.
	int id_persona = sala_teatro[id_asiento];
	sala_teatro[id_asiento]=-1;
	pthread_mutex_unlock(&cerrojo);
	return id_persona;

}

int estado_asiento(int id_asiento){
	//Falla si la sala no esta creada o si el id del asiento se sale del espacio.
	if(sala_teatro==NULL || id_asiento >= capacidad_total || id_asiento < 0) return -1;
	
	//El asiento esta vacío.
	if(sala_teatro[id_asiento] == -1) return 0;
	
	//Devuelve la persona que reservó el asiento.
	return sala_teatro[id_asiento];
}

int asientos_ocupados(){
    //Falla si la sala no esta creada o si el id del asiento se sale del espacio.
    if(sala_teatro == NULL){
        return -1;
    }
    int ocupados = 0;
    // Inicializamos el contador de asientos ocupados a 0
    for(int i=0; i<capacidad_total; i++){
        if(sala_teatro[i] != -1){
            // Si el asiento no esta libre, incrementamos la variable "ocupados"
            ocupados++;
        }
    }
    return ocupados;
}

int asientos_libres(){
	//Falla si la sala no esta creada.
	if(sala_teatro==NULL) return -1;
	return capacidad_total - asientos_ocupados();
}

int capacidad_sala(){
    //Falla si la sala no esta creada o si el id del asiento se sale del espacio.
    if(sala_teatro == NULL){
        return -1;
    }
    return capacidad_total;
}

int crea_sala(int capacidad){
    //Falla si la sala no esta creada o si el id del asiento se sale del espacio.
    if(sala_teatro != NULL || capacidad < 1){
        return -1;
    }
    capacidad_total = capacidad;
    // Creamos un puntero en memoria, reservando espacio para almacenar
    // los asientos de la sala. Lo ajustamos en base al tamaño del entero.
    sala_teatro = (int*)malloc(capacidad*sizeof(int));
    for(int i=0; i<capacidad; i++){
    	// A cada asiento de la sala le asignamos -1 para indicar de que estan libres
        sala_teatro[i] = -1;
    }
    return capacidad;
}

int elimina_sala(){
    //Falla si la sala no esta creada o si el id del asiento se sale del espacio.
    if(sala_teatro == NULL){
        return -1;
    }
    // Liberamos el espacio reservado en memoria y inicializamos nuevamente "sala_teatro" a NULL.
    free(sala_teatro);
    sala_teatro=NULL;
    return 0;
}

void comprobar_asientos(){
	if (sala_teatro == NULL) {
		fprintf(stderr, "La sala no está creada.\n");
		return;
	}
	printf("Asientos totales: %d\nAsientos ocupados: %d\nAsientos libres: %d\n",
		   capacidad_sala(), asientos_ocupados(), asientos_libres());
	for (int i = 0; i < capacidad_total; i++) {
		if (sala_teatro[i] == -1) {
			printf("Asiento %3dº: Libre\n", i);
		} else {
			printf("Asiento %3dº: Ocupado por %d\n", i, sala_teatro[i]);
		}
	}
}

