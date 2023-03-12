#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include "implementacion.h"
#define peticion_root "../peticion/" // raiz para coger los ficheros
#define formato_fichero ".dat"      // definimos el formato de fichero. En este caso, extension .dat


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
            //Borrarmos todo el contenido
            remove(ent->d_name);
        
    }

    closedir(dir);

    return 0;
}

int set_value(struct tupla_pet peticion) {

    FILE *fichero_peticion;
    char str_key[20];
    char nombre_fichero[50];

    sprintf(str_key, "%d", peticion.clave);
    sprintf(nombre_fichero, "%s%s%s", peticion_root, str_key, formato_fichero);
    
    fichero_peticion = fopen(nombre_fichero, "wb");

    if (fichero_peticion != NULL) {
        fwrite(&peticion, sizeof(struct tupla_pet), 1, fichero_peticion); 
        fclose(fichero_peticion);  
    } else {
        printf("No se pudo abrir el archivo.\n");
    }

   return 0;
}

struct tupla_pet get_value(struct tupla_pet tupla){
    //Esta función devuelve un struct que representa los valores de la clave key

    //creamos el struct que vamos a devolver
    struct tupla_pet get;


    char str_key[20];
    char peticion[50];
    char nombre_fichero[50];

    sprintf(str_key, "%d", tupla.clave);
    sprintf(nombre_fichero, "%s%s%s", peticion_root, str_key, formato_fichero);

    // Comprobamos si no hay existencia del fichero
    if (access(nombre_fichero, F_OK) != 0) {
        perror("La clave no existe");
    }

   FILE *archivo = fopen(nombre_fichero, "rb");  // Abrir el archivo para lectura binaria
    if (archivo != NULL) {
        fread(&get, sizeof(struct peticion_get), 1, archivo);  // Leer la estructura desde el archivo
        fclose(archivo);  // Cerrar el archivo
    } else {
        printf("No se pudo abrir el archivo.\n");
    }

    return get;
}



int modify_value(struct tupla_pet peticion) {
    //Esta función modifica el fichero que representa la clave key con los nuevos valores
/*
    FILE *fichero_peticion;
    char str_key[20];
    char peticion[50] = {0};

    sprintf(str_key, "%d", key);
    sprintf(peticion, "%s%s%s", peticion_root, str_key, formato_fichero);
    
*/
    return 0;

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
    printf("El fichero sí existe");
    return 1;
}
