#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "comunicacion.h"
#include "implementacion.h"

#define SERVIDOR "/SERVIDOR34"

mqd_t  q_servidor;


//mutex
pthread_mutex_t mutex_mensaje;
int mensaje_no_copiado;
pthread_cond_t cond_mensaje;

void tratar_mensaje(void *mess) 
{
    struct peticion mensaje;	        //mensaje local 
    struct respuesta respuesta;	        // respuesta local 
	mqd_t q_cliente;		            // cola del cliente 
    
    int resultado;		                // resultado de la operación 

    pthread_mutex_lock(&mutex_mensaje);

    //copia la petición a la variable mensaje
    mensaje = (*(struct peticion *) mess);
    
    //Como ya se ha copiado el mensaje, despetarmos al servidor 
    mensaje_no_copiado = false;

    pthread_cond_signal(&cond_mensaje);

	pthread_mutex_unlock(&mutex_mensaje);
    
    //leemos y ejecutamos la petición

    
    if (mensaje.c_op == 0) {//init
    fprintf(stderr, "\n\n Me meto porque me toca init\n");
        resultado = init_implementacion();}
   if (mensaje.c_op  == 1)
        resultado = set_value_implementacion(mensaje.tupla_peticion);
     if (mensaje.c_op == 2){ //get 
        resultado = get_value_implementacion(mensaje.tupla_peticion, &respuesta); 
        //imprimir valores devueltos a la tupla respuesta
        //fprintf(stderr, "El valor de la clave es %d %s %d %f\n", respuesta.tupla_peticion.clave, respuesta.tupla_peticion.valor1, respuesta.tupla_peticion.valor2, respuesta.tupla_peticion.valor3);
     }
    if (mensaje.c_op  == 3) //mod
        resultado = modify_value_implementacion(mensaje.tupla_peticion);
    if (mensaje.c_op == 4) //del
        resultado = delete_key_implementacion(mensaje.tupla_peticion.clave);
    if (mensaje.c_op  == 5) //exist
       resultado = exist_key_implementacion(mensaje.tupla_peticion.clave);
    if (mensaje.c_op  == 6) //copy
       resultado = copy_key_implementacion(mensaje.tupla_peticion.clave, mensaje.clave2);
    
    fprintf(stderr, "El valor del resultado es %d\n", resultado);
    respuesta.code_error = resultado;
    //se devuelve el resultado al cliente enviándolo a su cola
    q_cliente = mq_open(mensaje.q_name, O_WRONLY);

	if (q_cliente == -1)
    {
		perror("No se puede abrir la cola del cliente");
		mq_close(q_servidor);
		mq_unlink(SERVIDOR);
	}

    else 
    {
        
		if (mq_send(q_cliente, (char *)&respuesta, sizeof(struct respuesta), 0) <0) 
        {
			perror("tratar_mensaje(): mq_send");
			mq_close(q_servidor);
			mq_unlink(SERVIDOR);
			mq_close(q_cliente);
		}
        
        else
        {
            printf("Respuesta enviada al cliente\n");
        }
	}
	pthread_exit(0);
}




int main(void){
    struct peticion mess;
    struct mq_attr attr;

	pthread_attr_t t_attr;		// atributos de los threads 
   	pthread_t thid;


    attr.mq_maxmsg = 5;                
	attr.mq_msgsize = sizeof(struct peticion);
    
    //se crea y abre la cola del servidor
    q_servidor = mq_open(SERVIDOR, O_CREAT|O_RDONLY, 0700, &attr);
    if (q_servidor == -1) {
            perror("mq_open");
            return -1;
        }

	pthread_mutex_init(&mutex_mensaje, NULL);
	pthread_cond_init(&cond_mensaje, NULL);
	pthread_attr_init(&t_attr);

	// atributos de los threads, threads independientes que no necesitan hacer un join al final
	pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);
    mensaje_no_copiado = true;

    while(1) {
        
        if (mq_receive(q_servidor, (char *) &mess, sizeof(struct peticion), 0) < 0 ){
			perror("mq_recev");
			return -1;
		}
        //imprir código de operación recibido:
        printf("Código de operación recibido: %d, cola del cliente: %s\n", mess.c_op, mess.q_name);

        if (pthread_create(&thid, &t_attr, (void *)tratar_mensaje, (void *)&mess) == 0) {
            // se espera a que el thread copie el mensaje 
			pthread_mutex_lock(&mutex_mensaje);
			while (mensaje_no_copiado)
				pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
			mensaje_no_copiado = true;
			pthread_mutex_unlock(&mutex_mensaje);
        }
			
    }

    return 0;
    
}