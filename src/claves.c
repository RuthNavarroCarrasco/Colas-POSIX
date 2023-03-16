#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "comunicacion.h"
#include "claves.h"

#define peticion_root "../peticion/" // raiz para coger los ficheros
#define formato_fichero ".dat"      // definimos el formato de fichero. En este caso, extension .dat


int send_recieve(struct peticion *peticion) {
    int ret;
    char q_client[1024];
    struct mq_attr attr;        // atributos de la cola
    int qs, qc;                
    struct respuesta *respuesta;

    /* Inicializar los atributos de la cola */
    attr.mq_flags   = 0 ;
    attr.mq_maxmsg  = 10 ;
    attr.mq_msgsize = sizeof(struct peticion) ;


    qs = mq_open("/SERVIDOR", O_WRONLY) ;  //abrir cola del servidor
    if (qs == -1) 
    {
        perror("mq_open: ") ;
        return -1 ;
    }

    else {
        printf("Cola del servidor abierta correctamente\n");}


    //sprintf(q_client, "%s", peticion->q_name) ;
    strcpy(q_client, peticion->q_name);
    qc = mq_open(q_client, O_CREAT|O_RDONLY, 0664, &attr) ;  // se abre la cola del cliente
    if (qc == -1) 
    {
        perror("mq_open: ") ;
        mq_close(qs) ;
        return -1;
    }

    else{
        printf("Cola  %s del cliente abierta correctamente\n", q_client);
    }
    //strcpy(peticion->q_name, q_client);
    

    ret = mq_send(qs, (char *)&peticion, sizeof(struct peticion), 0) ; //enviar petición al servidor
    if (ret < 0) 
    {   
        
        perror("mq_send: ") ;
        mq_close(qs) ; // en caso de error cerramos cola del servidor y del cliente
        mq_close(qc) ;
        mq_unlink(q_client) ;
        return -1;
    }

    else{
        printf("Peticion enviada correctamente al servidor\n");}


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

     ret = mq_unlink(q_client);
     if (ret < 0) 
     {
	    perror("mq_unlink: ") ;
     }

    return respuesta->code_error;
}



int init() {
    struct peticion peticion = {0};
    peticion.c_op = 0;

    int code_error = send_recieve(&peticion);
    return code_error;
}

int set_value(int key, char *value1, int *value2, double value3) {
    //Esta función crea la petición con el código de operación correspondiente a INIT y llama a send_receive para enviarla y recibir la respuesta
    
    //creamos la peticion 

    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    strcpy(peticion.tupla_peticion.valor1, value1);
    peticion.tupla_peticion.valor2 = *value2;
    peticion.tupla_peticion.valor3 = value3;
    strcpy(peticion.q_name, "/cliente1");
    
    peticion.c_op = 1;
    int code_error = send_recieve(&peticion);

    return code_error;
}


int get_value(int key, char *value1, int *value2, double value3) {
    //creamos la peticion 
    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    strcpy(peticion.tupla_peticion.valor1, value1);
    peticion.tupla_peticion.valor2 = *value2;
    peticion.tupla_peticion.valor3 = value3;
    peticion.c_op = 2;
    int code_error = send_recieve(&peticion);

    return code_error;
}

int modify_value(int key, char *value1, int *value2, double value3){
    //creamos la peticion 

    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    strcpy(peticion.tupla_peticion.valor1, value1);
    peticion.tupla_peticion.valor2 = *value2;
    peticion.tupla_peticion.valor3 = value3;
    peticion.c_op = 3;
    int code_error = send_recieve(&peticion);

    return code_error;
}


int delete_key(int id) {
    //creamos la peticion 
    struct peticion peticion = {
        .tupla_peticion.clave = id,
        .c_op = 4
    };

    int code_error = send_recieve(&peticion);

    return code_error;
}



int exist_key(int id) {
    struct peticion peticion = {
        .tupla_peticion.clave = id,
        .tupla_peticion.valor2 = 0,
        .c_op = 5
    };
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
