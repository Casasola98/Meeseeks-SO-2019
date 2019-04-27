#ifndef ESTRUCTURAS
#define ESTRUCTURAS

#include <semaphore.h>

/*
Struct: infoMeeseek

Estructura que almacena los datos relevantes de un meeseek, los cuales son los
siguientes:
    - pid: process id del meeseek
    - ppid: process id del padre del meeseek
    - N: Nivel del arbol de meeseeks en el cual se encuentra
    - i: Numero de instancia del meeseek

Tipicamente esta estructura se usa para almacenar la info del meeseek que logro
solucionar el problema.
*/
struct infoMeeseek
{
    int *pid;
    int *ppid;
    int *N;
    int *i;
};

/*
Struct: globales

Estructura que almacena todas las variables de datos que comparten los procesos 
entre si para la ejecucion. No se incluye el mensaje del trabajo a realizar 
debido a las restricciones establecidas en la especificacion.

Para las variables de concluido, instancia y solucionador existen semaforos para
controlar su modificacion.
*/
struct globales{
    sem_t sem_concluido;
    //sem_t sem_instancia;
    sem_t sem_solucionador;

    int concluido;
    //int instancia;
    struct infoMeeseek *infoSolucionador;

    int instanciasFinalizadas;
};

#endif