#include <studio.h>
#include <unistd.h>
#include <dirent.h>
#include "implementacion.h"
#define peticion_root "../peticion/" // raiz para coger los ficheros
#define formato_fichero ".txt"


int init(){
    //Esta función borra todos los ficheros que representan las claves en el directorio peticion_root
    DIR *dir;
    struct dirent *ent;
    dir = opendir(peticion_root);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return -1;
    }

    //Recorremos el directorio
    while ((ent = readdir(dir)) != NULL) {
        //Si es un fichero
        if (ent->d_type == DT_REG) {
            //Lo borramos
            remove(ent->d_name);
        }
    }
}

int set_value(int key, char *value1, int value2, double value3) {
    
    //Esta función crea un fichero que representa la clave key con los valores value1, value2 y value3
    FILE *fichero_peticion;
    char str_key[20];
    char peticion[50] = {0};

    sprintf(str_key, "%d", key);
    sprintf(peticion, "%s%s%s", peticion_root, str_key, formato_fichero);

    // Comprobamos si ya hay existencia del fichero
    if (access(peticion, F_OK) == 0) {
        perror("La clave ya existe");
        return -1;
    }

    fichero_peticion = fopen(peticion, "w");
    if (fichero_peticion == NULL) {
        perror("Error al abrir el fichero");
        return -1;
    }

    //escribimos en el fichero
    fprintf(fichero_peticion, "value1: %s\nvalue2: %d\nvalue3: %f", value1, value2, value3);

    //cerramos el fichero
    fclose(fichero_peticion);
    return 0;
    
}

int get_value(int key, char *value1, int value2, double value3){
    //Esta función devuelve el valor de las valores de la clave key

    FILE *fichero_peticion;
    char str_key[20];
    char peticion[50];

    sprintf(str_key, "%d", key);
    strcpy(peticion, peticion_root);
    strcat(peticion, str_key);

    //char* value1
    //int value2
    //double value3; no del mismo type
    // Comprobamos si no hay existencia del fichero
    if (access(peticion, F_OK) != 0) {
        perror("La clave no existe");
        return -1;
    }
    
    //Abrimos el fichero
    fichero_peticion = fopen(peticion, "r");
    if (fichero_peticion == NULL) {
        perror("Error al abrir el fichero");
        return -1;
    }

    return 0;
}


int modify_value(int key, char *value1, int value2, double value3) {
    //Esta función modifica el fichero que representa la clave key con los nuevos valores

    FILE *fichero_peticion;
    char str_key[20];
    char peticion[50] = {0};

    sprintf(str_key, "%d", key);
    sprintf(peticion, "%s%s%s", peticion_root, str_key, formato_fichero);
    


}

int delete_key(int key){
    //esta función elimina el fichero que representa la clave key
    FILE *fichero_peticion;
    char str_key[20];
    char peticion[50];
    
    sprintf(str_key, "%d", key);
    strcpy(peticion, peticion_root);
    strcat(peticion, str_key);

    // Comprobamos si hay existencia del fichero
    if (access(peticion, F_OK) != 0) {
        perror("El fichero no existe");
        return -1;
    }

    //eliminamos el fichero
    if (remove(peticion) == 0) {
        printf("Fichero eliminado correctamente");
    } else {
        perror("Error al eliminar el fichero");
        return -1;
    }

}

int exist (int key){
    //Esta función devuelve 1 si existe el fichero que representa la clave key y 0 en caso contrario
    FILE *fichero_peticion;
    char str_key[20];
    char peticion[50];

    sprintf(str_key, "%d", key);
    strcpy(peticion, peticion_root);
    strcat(peticion, str_key);

    // Comprobamos si no hay existencia del fichero
    if (access(peticion, F_OK) != 0) {
        printf("El fichero no existe");
        return 0;
    }
    printf("El fichero sí existe")
    return 1;
}
