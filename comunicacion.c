/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "estructuras.h"

struct globales* compartirGlobales(char *segmentoMemoria, struct globales *vglobales){
    key_t key;
    int shmid;

    key = ftok("shmfile", 65);
    shmid = shmget(key, sizeof(vglobales), 0666|IPC_CREAT); //Obtiene segmento de mem compartida
    segmentoMemoria = shmat(shmid, (void *) 0, 0); //Pega el proceso al segmento de mem compartida
    vglobales = (struct globales*) segmentoMemoria; //Ahora vglobales es compartida

    //Se inicializan las variables
    vglobales->concluido = 777;

    vglobales->caos_planetario = 0;

    vglobales->pid = 0;
    vglobales->ppid = 0;
    vglobales->N = 0;
    vglobales->i = 0;

    vglobales->instanciasFinalizadas = 0;

    return vglobales;
}

void liberarGlobales(char *segmentoMemoria){
    shmdt(segmentoMemoria);
}

void init_semaforos(struct globales *vglobales){
   sem_init(&(vglobales->sem_concluido), 0, 1);
   //sem_init(&vglobales->sem_instancia, 1, 1);
   //sem_init(&vglobales->sem_solucionador, 0, 1);
}

void modificarConcluido(struct globales *vglobales, int valor){
    sem_wait(&vglobales->sem_concluido);
    vglobales->concluido = valor;
    sem_post(&vglobales->sem_concluido);
}

void modificarCaos(struct globales *vglobales, int valor){
    sem_wait(&vglobales->sem_caos_planetario);
    vglobales->caos_planetario = valor;
    sem_post(&vglobales->sem_caos_planetario);
}

/*void modificarInstancia(int *instancia, struct globales *vglobales){
    sem_wait(&vglobales->sem_instancia);
    vglobales->instancia +=1;
    *instancia = vglobales->instancia;
    sem_post(&vglobales->sem_instancia);

}*/

void modificarInformacionSolucionador(int pid, int ppid, int N, int i, struct globales *vglobales){
    vglobales->pid = pid;
    vglobales->ppid = ppid;
    vglobales->N = N;
    vglobales->i = i; 
}

void setMensajeEnTuberia(int *fd, char *tarea){
    close(fd[0]);
    write(fd[1], tarea, sizeof(tarea));
    close(fd[1]);
}

char* recibirMensajeDeTuberia(int *fd){
    char buffer[100];
    close(fd[1]);
    read(fd[0], buffer, sizeof(buffer));
    close(fd[0]);

    return buffer;
}