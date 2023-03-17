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

  /*  code_error = init();
    if (code_error < 0) 
    {
        printf("init(): código de error %d\n", code_error);
    }*/

    code_error = set_value(clave, valor1, valor2, valor3);
    if (code_error < 0) 
    {
        printf("set_value(): código de error %d\n", code_error);
    }

    
    code_error = get_value(clave, valor1, valor2, valor3);
    if (code_error < 0) 
    {
        printf("get_value(): código de error %d\n", code_error);
    }

    code_error = modify_value(clave, valor1, valor2, valor3);
    if (code_error < 0) 
    {
        printf("modify_value(): código de error %d\n", code_error);
    }

   

    code_error = exist_key(clave);
    if (code_error < 0) 
    {
        printf("exist_key(): código de error %d\n", code_error);
    }

    
    
    
          
    return 0; 
}