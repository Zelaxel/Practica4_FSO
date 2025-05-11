// Escribe el id en un 'asiento' en memoria y devuelve su dirección.
// Da -1 si no hay espacio o sala. 
int reserva_asiento(int id_persona);

// Borra el id de un 'asiento' en memoria y devuelve el id eliminado.
// Da -1 si el asiento ya esta libre o no hay sala.
int libera_asiento(int id_asiento);

// Devuelve el id de la persona que ocupa el asiento, o 0 si esta libre.
// Da -1 si no hay sala.
int estado_asiento(int id_asiento);

// Devuelve la cantidad de asientos libres.
// Da -1 si no hay sala. 
int asientos_libres();

// Devuelve la cantidad de asientos ocupados.
// Da -1 si no hay sala.
int asientos_ocupados();

// Devuelve la cantidad de asientos en la sala.
// Da -1 si no hay sala.
int capacidad_sala();

// Reserva un espacio de memoria en función de la capacidad de asientos pedido y devuelve la capacidad de la sala.
// Da -1 si ya hay una sala creada.
int crea_sala(int capacidad);

// Libera el espacio reservado para la sala y devuelve 0 si todo va bien.
// Da -1 si no hay sala.
int elimina_sala();
