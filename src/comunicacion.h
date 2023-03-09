#define MAXSIZE 255
#define INIT 0
#define SET 1
#define GET 2
#define MOD 3
#define DEL 4
#define EXIST 5
#define COPY 6
//TODO: preguntar sobre los códigos de operación

struct tupla_pet {
    int clave;              /* clave */
    char valor1[MAXSIZE];   /* valor1 */
    int valor2;             /* valor2 */
    double valor3;          /* valor3 */
};


struct peticion {
    struct tupla_pet tupla_peticion;  /* tupla de la peticion */
    char q_name[MAXSIZE];   /* nombre de la cola cliente */
    int c_op;               /* codigo de operacion, 0 (INIT), 1 (SET), 2 (GET), 3 (MOD), 4 (DEL), 5 (EXIST), 6 (COPY) */
}


struct respuesta {
    int clave;              /* clave */
    char valor1[MAXSIZE];   /* valor1 */
    int valor2;             /* valor2 */
    double valor3;          /* valor3 */
}