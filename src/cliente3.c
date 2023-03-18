#define MAXSIZE 255
#include <stdio.h>
#include <stdlib.h>
#include "claves.h"

/*El cliente se encarga de enviar peticiones al servidor */

int clave = 45;              
char valor1[MAXSIZE] = "Envio Mensaje";   
int valor2 = 4;             
double valor3 = 1.72;
int clave2 = 25;



int main() 
{
    int code_error;

    
    code_error = init();
    if (code_error < 0) 
    {
        printf("set_value(): código de error %d\n", code_error);
    }
     
    return 0; 
}