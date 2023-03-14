#define MAXSIZE 255
#define INIT 0
#define SET 1
#define GET 2
#define MOD 3
#define DEL 4
#define EXIST 5
#define COPY 6

// Usamos esto para evitar problema de redefinición
#ifndef TUPLA_PET_H
#define TUPLA_PET_H

struct tupla_pet {
    int clave;              /* clave */
    char valor1[MAXSIZE];   /* valor1 */
    int valor2;             /* valor2 */
    double valor3;          /* valor3 */
};

#endif /* TUPLA_PET_H */


// Usamos esto para evitar problema de redefinición
#ifndef PETICION_H
#define PETICION_H

struct peticion {
    struct tupla_pet tupla_peticion;  /* tupla de la peticion */
    char q_name[MAXSIZE];   /* nombre de la cola cliente */
    int c_op;               /* codigo de operacion, 0 (INIT), 1 (SET), 2 (GET), 3 (MOD), 4 (DEL), 5 (EXIST), 6 (COPY) */
};

#endif /* PETICION_H */


// Usamos esto para evitar problema de redefinición
#ifndef RESPUESTA_H
#define RESPUESTA_H

struct respuesta {
    struct tupla_pet tupla_peticion;  /* tupla de la peticion */
    int code_error;         // código de error
};

#endif /* RESPUESTA_H */
