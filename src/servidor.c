#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "comunicacion.h"

mqd_t  q_servidor;

//mutex
pthread_mutex_t mutex_mensaje;
int mensaje_no_copiado = true;
pthread_cond_t cond_mensaje;

void tratar_mensaje(void *mess) {
    struct tupla_pet mensaje;	/* mensaje local */
	mqd_t q_cliente;		/* cola del cliente */
	int resultado;		/* resultado de la operación */

    pthread_mutex_lock(&mutex_mensaje);

    //copia la petición a la variable mensaje
    mensaje = (*(struct tupla_pet *) mess);
    //Como ya se ha copiado el mensaje, despetarmos al servidor 
    mensaje_no_copiado = false;

    pthread_cond_signal(&cond_mensaje);

	pthread_mutex_unlock(&mutex_mensaje);

    //leemos y ejecutamos la petición
    if (mensaje.c_op == 0); //init
        //resultado = blabla
        // Lo que está definido en claves.c
    if (mensaje.c_op  == 1);//set
        //resultado = bla bla
    if (mensaje.c_op == 2); //get 
        //resultado = blabla
    if (mensaje.c_op  == 3);//mod
        //resultado = bla bla
    if (mensaje.c_op == 4); //del
        //resultado = blabla
    if (mensaje.c_op  == 5);//exit
        //resultado = bla bla
    if (mensaje.c_op == 6); //copy
        //resultado = blabla


    //se devuelve el resultado al cliente enviándolo a su cola
    q_cliente = mq_open(mensaje.q_name, O_WRONLY);
	if (q_cliente == -1){
		perror("No se puede abrir la cola del cliente");
		mq_close(q_servidor);
		mq_unlink("/SERVIDOR_SUMA2");
	}
}



int main(void){
    struct tupla_pet mess;
    struct mq_attr attr;

	pthread_attr_t t_attr;		// atributos de los threads 
   	pthread_t thid;

    attr.mq_maxmsg = 10;                
	attr.mq_msgsize = sizeof(struct tupla_pet);
    
    //se crea y abre la cola del servidor
    q_servidor = mq_open("/SERVIDOR", O_CREAT|O_RDONLY, 0700, &attr);
    if (q_servidor == -1) {
            perror("mq_open");
            return -1;
        }

	pthread_mutex_init(&mutex_mensaje, NULL);
	pthread_cond_init(&cond_mensaje, NULL);
	pthread_attr_init(&t_attr);

	// atributos de los threads, threads independientes que no necesitan hacer un join al final
	pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);


    while(1) {
        
        if (mq_receive(q_servidor, (char *) &mess, sizeof(mess), 0) < 0 ){
			perror("mq_recev");
			return -1;
		}

        if (pthread_create(&thid, &t_attr, (void *)tratar_mensaje, (void *)&mess)== 0) {
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