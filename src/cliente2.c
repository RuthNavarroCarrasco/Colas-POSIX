#define MAXSIZE 255
#include <stdio.h>
#include <stdlib.h>
#include "claves.h"

/*El cliente se encarga de enviar peticiones al servidor */

int clave = 45;              
char valor1[MAXSIZE] = "mensaje de prueba";   
int valor2 = 3;             
double valor3 = 2.2;
int clave2 = 25;



int main() 
{
    int code_error;

    code_error = set_value(clave2, valor1, valor2, valor3);
    if (code_error < 0) 
    {
        printf("set_value(): código de error %d\n", code_error);
    } else {
        printf("set_value(): Se ha procesado correctamente la petición. Código de error: %d\n", code_error);
    }


    /*
    code_error = copy_key(clave, clave2);
    if (code_error < 0) 
    {
        printf("copy_key(): código de error %d\n", code_error);
    }

    */      
    return 0; 
}