
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "comunicacion.h"
#include "claves.h"

#define peticion_root "../peticion/" // raiz para coger los ficheros
#define formato_fichero ".dat"      // definimos el formato de fichero. En este caso, extension .dat

#define SERVIDOR "/SERVIDOR34"

int send_recieve(struct peticion *peticion) 
{
    int ret;
    char q_client[1024];
    struct mq_attr attr;        // atributos de la cola
    int qs, qc;                
    struct respuesta respuesta;

    /* Inicializar los atributos de la cola */
    //attr.mq_flags   = 0 ;
    attr.mq_maxmsg  = 10 ;
    attr.mq_msgsize = sizeof(struct respuesta) ;


    qs = mq_open(SERVIDOR, O_WRONLY) ;  //abrir cola del servidor
    if (qs == -1) 
    {
        perror("mq_open SERVIDOR: ") ;
        return -1 ;
    }



    //sprintf(q_client, "%s", peticion->q_name) ;
    
    qc = mq_open( peticion->q_name, O_CREAT|O_RDONLY, 0664, &attr) ;  // se abre la cola del cliente
    if (qc == -1) 
    {
        perror("mq_open CLIENTE: ") ;
        mq_close(qs) ;
        return -1;
    }

    
    //strcpy(peticion->q_name, q_client);
    

    ret = mq_send(qs, (char *)peticion, sizeof(struct peticion), 0) ; 
    if (ret < 0) 
    {   
        
        perror("mq_send: ") ;
        mq_close(qs) ; // en caso de error cerramos cola del servidor y del cliente
        mq_close(qc) ;
        mq_unlink(q_client) ;
        return -1;
    }

    


    ret = mq_receive(qc, (char *)&respuesta, sizeof(struct respuesta), 0) ; //recibir respuesta del servidor
    if (ret < 0) 
    {
        perror("mq_receive: ") ;
        mq_close(qs) ;
        mq_close(qc) ;
        mq_unlink(q_client) ;
        return -1;
    }

    // Cerramos las colas y eliminamos la cola del cliente
     ret = mq_close(qs);
     if (ret < 0) 
     {
	    perror("mq_close: ") ;
     }

     ret = mq_close(qc);
     if (ret < 0) 
     {
	    perror("mq_close: ") ;
     }

     ret = mq_unlink(peticion->q_name);
     if (ret < 0) 
     {
	    perror("mq_unlink: ") ;
     }

    return respuesta.code_error;
}



int init() 
{
    struct peticion peticion = {0};
    peticion.c_op = 0;

    int code_error = send_recieve(&peticion);
    return code_error;
}

int set_value(int key, char *value1, int value2, double value3) 
{
    //Esta función crea la petición con el código de operación correspondiente a INIT y llama a send_receive para enviarla y recibir la respuesta
    
    //creamos la peticion 

    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    strcpy(peticion.tupla_peticion.valor1, value1);
    peticion.tupla_peticion.valor2 = value2;
    peticion.tupla_peticion.valor3 = value3;
    sprintf(peticion.q_name, "/cliente_%d", getpid());
    
    peticion.c_op = 1;
    int code_error = send_recieve(&peticion);

    return code_error;
}


int get_value(int key, char *value1, int value2, double value3) 
{
    //creamos la peticion 
    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    strcpy(peticion.tupla_peticion.valor1, value1);
    peticion.tupla_peticion.valor2 = value2;
    peticion.tupla_peticion.valor3 = value3;
    peticion.c_op = 2;

    sprintf(peticion.q_name, "/cliente_%d", getpid());
    
    int code_error = send_recieve(&peticion);

    return code_error;
}


int modify_value(int key, char *value1, int value2, double value3)
{
    //creamos la peticion 

    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    strcpy(peticion.tupla_peticion.valor1, value1);
    peticion.tupla_peticion.valor2 = value2;
    peticion.tupla_peticion.valor3 = value3;
    peticion.c_op = 3;
    sprintf(peticion.q_name, "/cliente_%d", getpid());

    int code_error = send_recieve(&peticion);

    return code_error;
}


int delete_key(int id) 
{
    //creamos la peticion 
    struct peticion peticion = {
        .tupla_peticion.clave = id,
        .c_op = 4
    };
    sprintf(peticion.q_name, "/cliente_%d", getpid());


    int code_error = send_recieve(&peticion);

    return code_error;
}



int exist_key(int id) 
{
    struct peticion peticion = {
        .tupla_peticion.clave = id,
        .tupla_peticion.valor2 = 0,
        .c_op = 5
    };

    sprintf(peticion.q_name, "/cliente_%d", getpid());

    int code_error = send_recieve(&peticion);

    return code_error;
}


int copy_key(int key1, int key2) 
{   
    struct peticion peticion = {
        .tupla_peticion.clave = key1,
        .clave2 = key2,
        .c_op = 6
    };
    int code_error = send_recieve(&peticion);

    return code_error;
}
