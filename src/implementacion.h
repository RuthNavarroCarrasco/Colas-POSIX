/* Servicios*/
#include "comunicacion.h"

int init_implementacion();

int set_value_implementacion(struct tupla_pet peticion);

struct tupla_pet get_value_implementacion(struct tupla_pet peticion);

int modify_value_implementacion(struct tupla_pet peticion);

int delete_key_implementacion(int key);

int exist_implementacion(int key);

int copy_key_implementacion(int key1, int key2);