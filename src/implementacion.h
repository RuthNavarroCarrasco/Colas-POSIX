/* Servicios*/
#include "comunicacion.h"

int init();

int set_value(struct tupla_pet peticion);

int get_value(struct tupla_pet peticion);

int modify_value(struct tupla_pet peticion);

int delete_key(int key);

int exist(int key);

int copy_key(int key1, int key2);