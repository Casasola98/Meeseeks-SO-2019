#ifndef ESTRUCTURAS
#define ESTRUCTURAS

#include <semaphore.h>

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
    sem_t sem_caos_planetario;
    //sem_t sem_instancia;
    //sem_t sem_solucionador;

    int concluido;
    int caos_planetario;

    int pid;
    int ppid;
    int N;
    int i;
    //int instancia;
    
    int instanciasFinalizadas;
};

#endif