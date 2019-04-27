/*#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <time.h>*/
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

#include "meeseeks_forks.c"
#include "estructuras.h"
//#include "operaciones.c"

int main(){
    srand (time(NULL)); //Inicializa el seed del rand

    char n;
    char* tarea;
    double dificultad;
    char* bitacora = malloc(sizeof(char)*1048576);

    int MeeseekBox = getpid();
    printf("Meeseeks Box    %d \n", MeeseekBox);
    
    while(n != '4'){
        impMeeseekBox();
        printf("Seleccione la accion que desea realizar:\n    1. Accion Textual\n    2. Operaciones Aritmeticas\n    3. Ejecutar Programa\n    4. Salir\n\nIngrese su opcion: ");

        scanf("%c", &n);
        getchar();
        
        char* actividad;

        switch (n)
        {
        case '1':
            tarea = leerSolicitud();
            dificultad = leerDificultad();

            actividad = malloc(sizeof(char)*1000);
            strcat(actividad, iniciar(tarea, dificultad));
            
            break;
        case '2':
            actividad = malloc(sizeof(char)*1000);
            strcat(actividad, operacionAritmetica());
            break;
        case '3':
            actividad = malloc(sizeof(char)*1000);
            strcat(actividad,  ejecutarPrograma());
            break;
        case '4':
            printf("Concluyendo Mr. Meeseeks Box, hasta luego\n");
            break;
        default:
            printf("Opcion inexistente en la Mr. Meeseeks Box  %c %d\n", n, getpid());
            break;
        }

        if(MeeseekBox != getpid()){
            break;
        }
        else{
            //guarda la actividad en la bitacora solo si es el fork de la caja
            //por eso lo guardo hasta aqui abajo
            strcat(bitacora, actividad);
        }
    }
    return 0;
}