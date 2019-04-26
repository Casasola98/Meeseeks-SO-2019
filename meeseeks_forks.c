#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "estructuras.h"
#include "comunicacion.c"
#include "operaciones.c"

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

    if(dificultad == -1){
        dificultad = generarRandom(0, 100);
        printf("Mr. Meeseeks dice que la dificultad es: %d\n", dificultad);
    }else
    {
        printf("Mr. Meeseeks hara la tarea con dificultad: %d\n", dificultad);
    }
    
    return dificultad;
}

double diluirDificultad(double dificultad){
    if(dificultad == 0){
        return dificultad;
    } else{
        double temp = generarRandom(1, (int) dificultad); //Random no mayor a la dificultad
        double reduc = temp * (dificultad / 1000); //Se reduce a una milesima dictada por la dificultad
        
        return dificultad + reduc;
    }
}

bool intentarTarea(double dificultad){
    int milisegundos = generarRandom(500, 5000); //Para dormir entre 0.5s y 5s

    //El Mr. Meeseeks 'intenta' la tarea por un tiempo
    usleep(milisegundos * 1000); //Conversion a microsegundos (u)

    if(dificultad <= 85.01){
        return false; //Necesita ayuda de otros Mr. Meeseeks
    } else{
        return true; //Logro resolver la tarea facil
    }
}

int obtenerHijosPorCrear(double dificultad){
    if (dificultad >= 0 & dificultad <= 45){
        return 3; //TODO ver si hacemos que cree n meeseeks :v
        
    }else if(dificultad > 45 & dificultad <= 85){
        //Crea entre uno y dos Meeseeks
        return generarRandom(1, 3);

    }else{
        return 0;
    }
}

void iniciar(char* tarea, double dificultad){
    pid_t pid;

    int N = 1;
    int instancia = 1;
    int temp_instancia; //Para asignar la instancia a los hijos

    bool esHijo;

    clock_t inicio = clock();
    double tiempoTotal = 0.0;

    pid = fork();

    if(pid == 0){

        printf("Hi I'm Mr Meeseeks! Look at Meeeee. (pid: %d, ppid: %d, N: %d, i: %d)\n",
            getpid(),
            getppid(),
            N,
            instancia
        );

        while(true){
            //esHijo = false;

            if(intentarTarea(dificultad)){
                //Resuelve y notifica al padre para matar a los hijos
                Bold_Blue();
                printf("HE FINALIZADO (pid: %d, ppid: %d, N: %d, i: %d) \n", //Temporal
                        getpid(), 
                        getppid(), 
                        N, 
                        instancia
                );
                Reset_Color();
                break;
                //exit(0);
                
            }else{
                //Crea hijos
                int numHijos = obtenerHijosPorCrear(dificultad);
                N++; //Incrementa el nivel

                for(int i = 0; i < numHijos; i++){
                    temp_instancia = i + 1;
                    pid = fork();

                    if(pid == 0){
                        //esHijo = true;
                        //Hace lo que hacen los nuevos Meeseeks
                        srand(time(NULL) ^ (getpid()<<16)); //Nueva semilla basada en el pid
                        instancia = temp_instancia;
                        printf(
                            "Hi I'm Mr Meeseeks! Look at Meeeee. (pid: %d, ppid: %d, N: %d, i: %d)\n",
                            getpid(),
                            getppid(),
                            N,
                            instancia
                        );

                        dificultad = diluirDificultad(dificultad);

                        break;
                    }
                    
                }

                if(pid != 0){
                    while (wait(NULL)>0)
                        {
                            //holi
                        }
                    break;
                }
            }
        }

    }
    else{
        wait(NULL);
        tiempoTotal = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        printf("The Mr Meeseeks lasted %f \n", tiempoTotal);
    }

    /*pid_t pid;
    int pipe_a_padre[2];

    int N = 1;
    int instancia = 1;
    int temp_instancia; //Para asignarle el num de instancia al hijo
    
    printf(
        "Hi I'm Mr Meeseeks! Look at Meeeee. (pid: %d, ppid: %d, N: %d, i: %d)\n",
        getpid(),
        getppid(),
        N,
        instancia
    );

    while(true){
        if(intentarTarea(dificultad)){
            printf("HE FINALIZADO (pid: %d, ppid: %d, N: %d, i: %d)", //Temporal
                    getpid(), 
                    getppid(), 
                    N, 
                    instancia
            );
            //Inicia algun procedimiento para matar a los meeseeks y finalizar
            exit(0);
        } else{
            int numHijos = obtenerHijosPorCrear(dificultad);
            //TODO: inicializar numHijos pipes
        
            for(int i = 0; i < numHijos; i++){
                temp_instancia = i;
                pid = fork();

                if(pid == 0) { 
                    //Hijo
                    srand(time(NULL) ^ (getpid()<<16)); //Nueva semilla basada en el pid
                    N++; //Incrementa el nivel
                    instancia = temp_instancia; //Asigna correctamente la instancia

                    printf(
                        "Hi I'm Mr Meeseeks! Look at Meeeee. (pid: %d, ppid: %d, N: %d, i: %d)\n",
                        getpid(),
                        getppid(),
                        N,
                        instancia
                    );

                    dificultad = diluirDificultad(dificultad);
                    break;
                    //printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid()); 
                    //exit(0); 
                } 
            }

            if(pid > 0){
                wait(NULL);
            }
            /*for(int i = 0; i < numHijos; i++){
                if(pid != 0){ //Es el padre, sigue creando
                    pid = fork();
                    printf("pid: %d", pid);
                    if(pid == 0){
                        printf("pid: %d, ppid: %d", getpid(), getppid());
                    }   
                } else{ //Es el hijo               
                    break;		
                }	
            }*/

            /*if(pid < 0){
                //Un error
            } else if(pid == 0){
                //Hijo
                /*srand(time(NULL) ^ (getpid()<<16)); //Nueva semilla basada en el pid
                N++; //Incrementa el nivel
                instancia = temp_instancia; //Asigna correctamente la instancia

                printf(
                    "Hi I'm Mr Meeseeks! Look at Meeeee. (pid: %d, ppid: %d, N: %d, i: %d)\n",
                    getpid(), 
                    getppid(), 
                    N, 
                    instancia
                );
                dificultad = diluirDificultad(dificultad);

            } else{
                wait(NULL);
                break;
            }
        }
    }*/
}