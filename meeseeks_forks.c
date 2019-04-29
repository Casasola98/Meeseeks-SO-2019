#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "estructuras.h"
#include "comunicacion.c"
#include "operaciones.c"

char *segmentoMemoria;
struct globales *vglobales;

/* generarRandom

Funcion que genera ints aleatorios entre inicio y fin. 

Puede ser utilizada para la generacion de la dificultad (0 a 100) o para el 
tiempo aleatorio que espera cada Meeseek antes de decidir si puede completar
la tarea o no (500 a 5000; en milisegundos).
*/
int generarRandom(int inicio, int fin){
    return (rand() % (fin - inicio + 1)) + inicio;
}

char* leerSolicitud(){
    char* tarea = malloc(sizeof(char)*1000);

    printf("Que necesita que haga Mr. Meeseeks? ");
    scanf("%[^\n]s", tarea);
    getchar();

    printf("Mr. Meeseeks hara: %s\n",tarea);

    return tarea;
}

int leerDificultad(){
    int dificultad;
    printf("Ingrese la dificultad de su solicitud (100:trivial, 0:imposible, -1:No sabe): ");
    scanf("%d", &dificultad);
    getchar();

    if(dificultad == -1){
        dificultad = generarRandom(0, 100);
        printf("Mr. Meeseeks dice que la dificultad es: %d\n", dificultad);
    }else
    {
        printf("Mr. Meeseeks hara la tarea con dificultad: %d\n", dificultad);
    }
    
    return dificultad;
}

double diluirDificultad(double dificultad, int numHIjos){
    if(dificultad == 0){
        return dificultad;
    } else{
        double temp = generarRandom(1, (int) dificultad); //Random no mayor a la dificultad
        double reduc = temp * (dificultad / generarRandom(350,550)); //Se reduce a una milesima dictada por la dificultad
        double extra = (reduc * (numHIjos));

        return dificultad + reduc + extra;
    }
}

bool intentarTarea(double dificultad){
    int segundos = generarRandom(1, 5); //Para dormir entre 1s y 5s

    //El Mr. Meeseeks 'intenta' la tarea por un tiempo
    sleep(segundos); 

    if(dificultad <= 85.01){
        return false; //Necesita ayuda de otros Mr. Meeseeks
    } else{
        return true; //Logro resolver la tarea facil
    }
}

int obtenerHijosPorCrear(double dificultad){
    if (dificultad >= 0 & dificultad <= 45){
        return generarRandom(1, 5); 
        
    }else if(dificultad > 45 & dificultad <= 85){
        //Crea entre uno y dos Meeseeks
        return generarRandom(1, 2);

    }else{
        return 0;
    }
}

char* iniciar(char* tarea, double dificultad){
    pid_t pid;

    vglobales = compartirGlobales(segmentoMemoria, vglobales);
    sem_init(&vglobales->sem_concluido, 1, 1);
    sem_init(&vglobales->sem_caos_planetario, 1, 1);

    int N = 1;
    int instancia = 1;
    int temp_instancia; //Para asignar la instancia a los hijos
    int primerMeeseek;

    int* pipe_a_padre;
    int* pipes_a_hijos;

    clock_t inicio = clock();
    double tiempoTotal = 0.0;

    modificarConcluido(vglobales, 0); //concluido = false

    pid = fork(); //Primer Meeseek que crea la caja

    if(pid == 0){
        primerMeeseek = getpid();
        setpgid(0,0);
        printf("Hi I'm Mr Meeseeks! Look at Meeeee. (pid: %d, ppid: %d, N: %d, i: %d)\n",
            getpid(),
            getppid(),
            N,
            instancia
        );

        while(true){

            if(intentarTarea(dificultad)){
                //Resuelve y notifica al padre para matar a los hijos
                if(vglobales->concluido == 0){
                    /*printf("HE FINALIZADO (pid: %d, ppid: %d, N: %d, i: %d) \n", //Temporal
                        getpid(), 
                        getppid(), 
                        N, 
                        instancia
                    );*/
                    
                    modificarInformacionSolucionador(getpid(), getppid(), N, instancia, vglobales);
                    modificarConcluido(vglobales, 1); //concluido = true
                }             
                Reset_Color();
                break;
                
            }else{
                //Crea hijos
                int numHijos = obtenerHijosPorCrear(dificultad);
                N++; //Incrementa el nivel

                if(vglobales->instanciasFinalizadas >= 9000){
                    modificarCaos(vglobales, 1); //Se declara caos planetario
                }

                pipes_a_hijos = malloc(sizeof(int)*numHijos);

                for(int i = 0; i < numHijos; i++){
                    temp_instancia = i + 1;
                    
                    int* pipe_temp = malloc(sizeof(int)*2);
                    pipe(pipe_temp);

                    pid = fork();

                    if(pid == 0){
                        //Hace lo que hacen los nuevos Meeseeks
                        vglobales->instanciasFinalizadas++;

                        srand(time(NULL) ^ (getpid()<<16)); //Nueva semilla basada en el pid
                        instancia = temp_instancia;
                        printf(
                            "Hi I'm Mr Meeseeks! Look at Meeeee. (pid: %d, ppid: %d, N: %d, i: %d)\n",
                            getpid(),
                            getppid(),
                            N,
                            instancia
                        );

                        //printf("Meeseeks (pid: %d) asignando pipe_a_padre\n", getpid());
                        pipe_a_padre = pipe_temp;

                        //Leer mensaje desde el pipe (la tarea)
                        char* mensaje = malloc(sizeof(char)*500);
                        close(pipe_a_padre[1]);
                        read(pipe_a_padre[0], mensaje, sizeof(mensaje));
                        close(pipe_a_padre[0]);

                        dificultad = diluirDificultad(dificultad, numHijos);
                        break;
                    }else{
                        //Guarda el pipe al hijo
                        pipes_a_hijos[i] = pipe_temp;

                        //Escribir la tarea a pipe_temp
                        close(pipe_temp[0]);
                        write(pipe_temp[1], tarea, sizeof(tarea));
                        close(pipe_temp[1]);
                    }
                }

                if(pid != 0){
                    while (wait(NULL) > 0){}
                    break;
                }
            }
        }
    
    }
    else{
        //Codigo de proceso Meeseeks Box original, aqui se cuentan los tiempos de ejecucion
        while(vglobales->concluido == 0 && vglobales->caos_planetario == 0){}
        tiempoTotal = (double)(clock() - inicio) / CLOCKS_PER_SEC;

        kill(-pid, SIGTERM);
        sleep(2);
        kill(-pid, SIGKILL);

        wait(NULL); 

        //Se fija si ha concluido o si hay caos
        if(vglobales->caos_planetario == 1){
            printf("ALERTA! SE HA DECLARADO CAOS PLANETARIO\n");
            impFailedTask();
        }else{
            impCloud( vglobales->pid, vglobales->ppid, vglobales->N, vglobales->i );
            impMeeseek();
            Reset_Color();
        }
        
        
        printf(
            "The %d Mr Meeseeks lasted %f seconds\n",  
            vglobales->instanciasFinalizadas,
            tiempoTotal
        );

        //Escribe el mensaje que va a contener la bitacora
        char* mensaje = malloc(sizeof(char)*1000);
        char* dif = malloc(sizeof(char)*1000);
        strcat(mensaje, "Consulta Textual: ");
        strcat(mensaje, tarea);
        strcat(mensaje, ", Dificultad: ");
        snprintf(dif, 50, "%f", dificultad);
        strcat(mensaje, dif);
        strcat(mensaje, "\n");
        return mensaje;
    }
}
